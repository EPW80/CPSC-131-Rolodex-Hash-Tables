# My Rolodex Hash Table

MyRolodex is a simplified, in-memory Rolodex implementation in C++. It is designed to store and manage a set of contacts, where each contact is represented by a person's name and their address information.

## Built

- C++

## MyRolodex

The MyRolodex class is the main class of the project. It has the following public methods:

- MyRolodex(): Constructor of the class, creates an empty rolodex.
- bool exists(std::string name) const: Returns true if a person with the given name exists in the Rolodex, false otherwise.
- void add(std::string name, std::string line1, std::string line2, std::string city, std::string state, int zip, std::string country): Adds a new contact to the Rolodex using the given name and address parts.
- void add(std::string name, Address address): Adds a new contact to the Rolodex using the given name and an Address object.
- Address get(std::string name) const: Returns the address of the person with the given name.
- void remove(std::string name): Removes the person with the given name from the Rolodex.
- void clear(): Clears all contacts from the Rolodex.
- bool empty(): Returns true if the Rolodex is empty, false otherwise.
- size_t size(): Returns the number of contacts in the Rolodex.

## Demo

![](./demo.gif)

## Contributor

Erik Williams
