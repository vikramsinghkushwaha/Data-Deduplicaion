# Data-Deduplicaion
Multiple copies of the same file ae not maintained and a pointer of that user to that location of the file is mainained.
Server and client architecture where multiple copies of the same file is not maintained whereas a pointer is maintained.
The file is hashed and the hash value is stored and maintained in a hashtable.
Whenever a new file is sent to server, hash value is checked from the hash table and then if not existing then as a new entry the hash and its location will be stored else pointer pointing to the same 
file will be maintained in the hashtable.
