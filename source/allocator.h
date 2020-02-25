#pragma once

#include <set>
#include <utility>

struct Chunk {
    size_t size;
    uint8_t *start_address;
    uint8_t *cur_address;

    Chunk() = default;

    Chunk(size_t size, void *start_address, void *cur_address) :
            size{size},
            start_address{reinterpret_cast<uint8_t *>(start_address)},
            cur_address{reinterpret_cast<uint8_t *>(cur_address)} {}

    Chunk(const Chunk &) = delete;

    Chunk(Chunk &&) = delete;
};

struct ChunkComparator {
    using is_transparent = void;

    bool operator()(size_t lhs, const Chunk &rhs) const {
        return lhs < rhs.size;
    }

    bool operator()(const Chunk &lhs, size_t rhs) const {
        return lhs.size < rhs;
    }

    bool operator()(const Chunk &lhs, const Chunk &rhs) const {
        return lhs.size < rhs.size;
    }
};

template<typename T, size_t N>
class ChunkPool {
public:
    ~ChunkPool() {
        for (auto &chunk: chunks_) {
            free(chunk.start_address);
        }
    }

    T *getFreeAddr(size_t n) {
        T *free_addr = nullptr;
        if (auto it = chunks_.lower_bound(n); it != chunks_.end()) {
            free_addr = decreaseChunk(it, n);
        }

        if (!free_addr) {
            auto it = addChunk(std::max(N, n));
            free_addr = decreaseChunk(it, n);
        }

        return free_addr;
    }

    template<typename It>
    T *decreaseChunk(It it, size_t n) {
        auto node = chunks_.extract(it);
        auto &chunk = node.value();
        T *free_addr = reinterpret_cast<T *>(chunk.cur_address);

        chunk.size -= n;
        chunk.cur_address += n;
        chunks_.insert(std::move(node));

        return free_addr;
    }

    auto addChunk(size_t n) {
        T *start_address = reinterpret_cast<T *>(malloc(n));
        if (!start_address) {
            throw std::bad_alloc();
        }

        return chunks_.emplace(n, start_address, start_address);
    }

private:
    std::multiset<Chunk, ChunkComparator> chunks_;
};

template<typename T, size_t N = 64>
class CustomAllocator {
public:
    using value_type = T;

    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    template<typename U>
    struct rebind {
        using other = CustomAllocator<U, N>;
    };

    CustomAllocator()=default;

    ~CustomAllocator() = default;

    template<typename U>
    CustomAllocator(const CustomAllocator<U, N> &) {
    }

    T *allocate(std::size_t n) {
        return chunk_pool_.getFreeAddr(n * sizeof(T));
    }

    void deallocate(T *p, std::size_t n) {
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) {
        p->~T();
    }

private:
    ChunkPool<T, N * sizeof(T)> chunk_pool_;
};