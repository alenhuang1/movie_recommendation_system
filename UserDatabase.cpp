#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

UserDatabase::UserDatabase()
{

}

UserDatabase::~UserDatabase(){
    std::vector<User*>::iterator it = m_userVector.begin();
    while(it != m_userVector.end()){
        delete *it;
        it++;
    }
}

bool UserDatabase::load(const string& filename)
{
    ifstream infile(filename);
    while(infile){
        std::string name;
        getline(infile,name);

        std::string email;
        getline(infile,email);

        int k;
        infile >> k;
        infile.ignore(10000,'\n');

        // Creates a vector of the user's watch history
        std::vector<std::string> watch_history;
        for(int i=0;i<k;i++){
            std::string movieID;
            getline(infile,movieID);
            watch_history.push_back(movieID);
        }

        // Creates a user object and inserts it into the user vector and the user map
        User* user = new User(name,email,watch_history);
        m_userVector.push_back(user);
        m_users.insert(email,user);
        infile.ignore(10000, '\n');
        if(infile.eof()){
            return true;
        }
    }
    return false;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<std::string,User*>::Iterator it = m_users.find(email);
    if(it.is_valid()){
        return it.get_value();
    }
    return nullptr;
}
