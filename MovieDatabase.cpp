#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

MovieDatabase::MovieDatabase()
{
    
}

MovieDatabase::~MovieDatabase(){
    std::vector<Movie*>::iterator it = m_allMovies.begin();
    while(it != m_allMovies.end()){
        delete *it;
        it++;
    }
}

bool MovieDatabase::load(const string& filename)
{
    ifstream infile(filename);
    while(infile){
        std::string movieID;
        getline(infile,movieID);

        std::string title;
        getline(infile,title);

        std::string year;
        getline(infile,year);
        
        // Creates a new vector of directors and pushes them into the vector
        std::vector<std::string> directorVector;
        std::string director;
        getline(infile,director);
        for(int i=0;i<director.size();i++){
            if(director[i] == ','){
                directorVector.push_back(director.substr(0,i));
                director = director.substr(i+1);
                i = 0;
            }
        }
        directorVector.push_back(director);

        //Creates a new vector of actors and pushes them into the vector
        std::vector<std::string> actorVector;
        std::string actor;
        getline(infile,actor);
        for(int i=0;i<actor.size();i++){
            if(actor[i] == ','){
                actorVector.push_back(actor.substr(0,i));
                actor = actor.substr(i+1);
                i = 0;
            }
        }
        actorVector.push_back(actor);

        //Creates a new vector of genres and pushes them into the vector
        std::vector<std::string> genreVector;
        std::string genre;
        getline(infile,genre);
        for(int i=0;i<genre.size();i++){
            if(genre[i] == ','){
                genreVector.push_back(genre.substr(0,i));
                genre = genre.substr(i+1);
                i = 0;
            }
        }
        genreVector.push_back(genre);

        float rating;
        infile >> rating;

        Movie* movies = new Movie(movieID,title,year,directorVector,actorVector,genreVector,rating);
        // Creates a new vector containing all the new movies created so they can be deleted later
        m_allMovies.push_back(movies);

        // Creates a new multitreemap with the movieID as the key and the movie as the value
        string temp = movieID;
        // Converts the key (id) to all lowercase so it can be used to find the movie
        for(int i = 0; i < temp.size();i++){
            temp.at(i) = tolower(temp.at(i));
        }
        m_movies.insert(temp,movies);
        
        // Creates a new multitreemap with the directors as the key and the movie as the value
        std::vector<std::string>::iterator it = directorVector.begin();
        while(it != directorVector.end()){
            string temp = *it;
            // Converts the key (director) to all lowercase so it can be used to find the movie
            for(int i = 0; i < temp.size();i++){
                temp.at(i) = tolower(temp.at(i));
            }
            m_directors.insert(temp,movies);
            it++;
        }

        // Creates a new multitreemap with the actors as the key and the movie as the value
        std::vector<std::string>::iterator it2 = actorVector.begin();
        while(it2 != actorVector.end()){
            string temp = *it2;
            // Converts the key (actor) to all lowercase so it can be used to find the movie
            for(int i = 0; i < temp.size();i++){
                temp.at(i) = tolower(temp.at(i));
            }
            m_actors.insert(temp,movies);
            it2++;
        }

        // Creates a new multitreemap with the genres as the key and the movie as the value
        std::vector<std::string>::iterator it3 = genreVector.begin();
        while(it3 != genreVector.end()){
            string temp = *it3;
            // Converts the key (genre) to all lowercase so it can be used to find the movie
            for(int i = 0; i < temp.size();i++){
                temp.at(i) = tolower(temp.at(i));
            }
            m_genres.insert(temp,movies);
            it3++;
        }

        infile.ignore(10000, '\n');
        infile.ignore(10000, '\n');

        if(infile.eof()){
            return true;
        }
    }
    return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    string temp = id;
    // Converts the input id to all lowercase so it can be used to find the movie
    for(int i = 0; i < temp.size();i++){
        temp.at(i) = tolower(temp.at(i));
    }
    TreeMultimap<std::string,Movie*>::Iterator it = m_movies.find(temp);
    if(it.is_valid()){
        return it.get_value();
    }
    return nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    string temp = director;
    // Converts the input director to all lowercase so it can be used to find the movie
    for(int i =0;i<temp.size();i++){
        temp.at(i) = tolower(temp.at(i));
    }
    TreeMultimap<std::string,Movie*>::Iterator it = m_directors.find(temp);
    vector<Movie*> directorMovies;
    while(it.is_valid()){
        directorMovies.push_back(it.get_value());
        it.advance();
    }
    return directorMovies;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    string temp = actor;
    // Converts the input actor to all lowercase so it can be used to find the movie
    for(int i =0;i<temp.size();i++){
        temp.at(i) = tolower(temp.at(i));
    }
    TreeMultimap<std::string,Movie*>::Iterator it = m_actors.find(temp);
    vector<Movie*> actorMovies;
    while(it.is_valid()){
        actorMovies.push_back(it.get_value());
        it.advance();
    }
    return actorMovies;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    string temp = genre;
    // Converts the input genre to all lowercase so it can be used to find the movie
    for(int i =0;i<temp.size();i++){
        temp.at(i) = tolower(temp.at(i));
    }
    TreeMultimap<std::string,Movie*>::Iterator it = m_genres.find(temp);
    vector<Movie*> genreMovies;
    while(it.is_valid()){
        genreMovies.push_back(it.get_value());
        it.advance();
    }
    return genreMovies;
}

