# Memstore

Memstore is a simple in-memory key-value store that takes some inspiration from Memcached. It's
intended to become a simple, fast, and reliable key-value store that can be used over a network.

***

## Contents

At the moment there are only two main parts of Memstore:

1. **Hash Table**: The hash table is the main structure that holds the key-value pairs. It's
   implemented as a hash table with quadratic probing for collision resolution. The table's
   size is always a power of two, and when the load factor exceeds 0.9, the table is resized
   to double its size.

2. **Slab Allocator**: The slab allocator is a memory allocator that divides memory into slabs
   of fixed sizes. Each slab is used to store a fixed value size for each key. The slab
   allocator is used to allocate memory for the hash table and the key-value pairs.
