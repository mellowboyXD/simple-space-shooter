## Implementing a custom hash table

I currently require a hash table in order to manage assets. What I had in mind
for the asset manager was to store the filename as key and asset ids as value.

#### Operations to be supported

- creating a hash table
- deleting a hash table
- adding key-value pairs to a hash table
- removing key-value pairs from a hash table
- finding a key in a hash table

#### Storage

The hash table will use a dynamic array as its storage space and reallocating
when 1/3 of the array is filled.

#### Collision Resolution

To keep things simple, I'll be using open [addressing](https://en.wikipedia.org/wiki/Open_addressing), more specifically, linear
probing to resolve hash collisions.


> *References*:
> - https://en.wikipedia.org/wiki/Open_addressing
