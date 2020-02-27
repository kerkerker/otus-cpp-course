#pragma once

template<typename T, typename Allocator = std::allocator<T>>
class CustomList
{
 public:
  class iterator;

  CustomList() = default;
  ~CustomList()
  {
    while (p_head_ != nullptr) {
      Node* next = p_head_->p_next;
      std::allocator_traits<NodeAllocator>::destroy(allocator_, p_head_);
      std::allocator_traits<NodeAllocator>::deallocate(allocator_, p_head_, 1);
      p_head_ = next;
    }
  }

  iterator begin()
  {
    return iterator(p_head_);
  }

  iterator end()
  {
    return iterator(nullptr);
  }

  void push_front(const T& elem)
  {
    Node* p_node = allocator_.allocate(1);
    allocator_.construct(p_node, elem);
    p_node->p_next = p_head_;
    p_head_ = p_node;
  }

 private:
  struct Node
  {
    template<typename... Args>
    Node(Args&& ... args): value(std::forward<Args>(args)...)
    {}
    T value;
    Node* p_next;
  };

  using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

  Node* p_head_ = nullptr;
  NodeAllocator allocator_;
};

template<typename T, typename Allocator>
class CustomList<T, Allocator>::iterator
{
 public:
  iterator(Node* p_node) : p_node_(p_node)
  {
  }

  iterator operator++()
  {
    if (p_node_) {
      p_node_ = p_node_->p_next;
    }
    return *this;
  }

  iterator operator++(int)
  {
    iterator prev = *this;
    if (p_node_) {
      p_node_ = p_node_->p_next;
    }
    return prev;
  }

  T& operator->()
  {
    return p_node_->value;
  }

  T& operator*()
  {
    return p_node_->value;
  }

  bool operator==(const iterator& rhs)
  {
    return p_node_ == rhs.p_node_;
  }

  bool operator!=(const iterator& rhs)
  {
    return !(*this == rhs);
  }

 private:
  Node* p_node_;
};