#include <string>
#include <iostream>
#include<fstream>
#include<sstream>
#include<bits/stdc++.h>
#include<sys/stat.h>
// #include <experimental/filesystem>
// #include "./sha256/sha256.h"
// #include "./sha256/sha.h"
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include  <iostream>
#include <bits/stdc++.h>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

using namespace std;

void  load(){
    ifstream ifs {"/home/vicky/socket/Minor/servercli/clientdetails/127.0.0.1.txt"};
    boost::archive::text_iarchive iarch(ifs);
    std::map<string, string> hashtable1;
    iarch >> hashtable1;
    cout<< hashtable1.size()<<endl;
    for (auto i = hashtable1.begin(); i != hashtable1.end(); ++i)
    {
        cout<<i->first<<" ";
        cout<<i->second<<endl;
    }
    // return hashtable1;
}

int main(int argc, char const *argv[])
{
    load();
    return 0;
} 