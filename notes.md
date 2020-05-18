####Inheritance:
The capability of a class to derive properties and characteristics from another class is called Inheritance.
Inheritance is one of the essential features of Object-Oriented Programming.

**Sub Class**:	The class that inherits properties from another class is called Sub Class or Derived Class.

**Super Class**:	The class whose properties are inherited by a Sub Class is called Base Class or Super Class.
A Sub Class can have several Super Classes, and similarly, a Super Class can have several Sub Classes.

####Priority Queue:
The priority queue (_the open set_) is maintained as a MinHeap binary tree.
It is stored in the form of a vector. In MinHeap, the parent node value is always less than it's children. So, the root node has the least value.
This property is the property used in the implementation of Dijkstra's algorithm.

#### The random library:
The '`random`' library in C++ is an excellent alternative for rand() function originated in C. It has many random functionalities even with specific probability distributions.
Although, the uniform distribution is the one used here. "`mt19937_64`" is one of the twister engines which takes a seed value.
By seeding with "`std::time( nullptr )`" from the '`ctime`' library, we can make it truly random.

Debugging is hell.


#### References used:
+ ["https://www.geeksforgeeks.org/"](https://www.geeksforgeeks.org/)
+ ["http://www.cplusplus.com/"](http://www.cplusplus.com/)