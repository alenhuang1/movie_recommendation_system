#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include "User.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database):m_user_database(user_database),m_movie_database(movie_database)
{
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    // Creates a copy of the user object from the user email
    User* user = m_user_database.get_user_from_email(user_email);
    std::unordered_map<string,int> m_movie_score;
    // Creates a copy of the user's watch history
    std::vector<string> watch_history = user->get_watch_history();
    std::vector<string>::iterator it = watch_history.begin();
    while(it!=watch_history.end()){
        // Creates a copy of the movie object from the movie id
        Movie* curr_movie = m_movie_database.get_movie_from_id(*it);
        // Creates a copy of the movie's actors
        std::vector<string> actors= curr_movie->get_actors();
        std::vector<string>::iterator it2 = actors.begin();
        while(it2!=actors.end()){
            // Creates a copy of the movies with the same actor
            std::vector<Movie*> movies_with_actor = m_movie_database.get_movies_with_actor(*it2);
            std::vector<Movie*>::iterator it3 = movies_with_actor.begin();
            while(it3!=movies_with_actor.end()){
                // Associates the movie id with a score, if it already exists in the map then add 30 to the score
                std::string movie_id = (*it3)->get_id();
                m_movie_score[movie_id] += 30;
                it3++;
            }
            it2++;
        }

        // Creates a copy of the movie's directors
        std::vector<string> directors= curr_movie->get_directors();
        std::vector<string>::iterator it4 = directors.begin();
        while(it4!=directors.end()){
            // Creates a copy of the movies with the same director
            std::vector<Movie*> movies_with_directors = m_movie_database.get_movies_with_director(*it4);
            std::vector<Movie*>::iterator it5 = movies_with_directors.begin();
            while(it5!=movies_with_directors.end()){
                // Associates the movie id with a score, if it already exists in the map then add 20 to the score
                std::string movie_id = (*it5)->get_id();
                m_movie_score[movie_id] += 20;
                it5++;
            }
            it4++;
        }

        // Creates a copy of the movie's genres
        std::vector<string> genres= curr_movie->get_genres();
        std::vector<string>::iterator it6 = genres.begin();
        while(it6!=genres.end()){
            // Creates a copy of the movies with the same genre
            std::vector<Movie*> movies_with_genres = m_movie_database.get_movies_with_genre(*it6);
            std::vector<Movie*>::iterator it7 = movies_with_genres.begin();
            while(it7!=movies_with_genres.end()){
                // Associates the movie id with a score, if it already exists in the map then add 1 to the score
                std::string movie_id = (*it7)->get_id();
                m_movie_score[movie_id] += 1;
                it7++;
            }
            it6++;
        }
        it++;
    }
    
    unordered_map<string,int>::iterator it8 = m_movie_score.begin();
    std::vector<MovieAndRank> recommendations;
    std::vector<MovieAndRank> real_recommendations;
    while(it8!=m_movie_score.end()){
        std::string movie_id = it8->first;
        // Checks if the movie is already in the user's watch history
        for(it = watch_history.begin();it!=watch_history.end();it++){
            if (movie_id == (*it)){
                it8++;
                break;
            }
        }
        if(it8 == m_movie_score.end()){
            break;
        }
        movie_id = it8->first;
        int score = it8->second;
        // Creates a vector of MoviesAndRank objects with movie ids and their scores
        recommendations.push_back(MovieAndRank(movie_id, score));
        it8++;
    }
    // Sorts the vector of MoviesAndRank objects using the compare struct
    std::sort(recommendations.begin(), recommendations.end(), compare(m_movie_database));
    // Checks if the movie count is negative, returns an empty vector if it is
    if(movie_count < 0){
        return real_recommendations;
    }
    
    for(int i = 0; i < movie_count;i++){
        real_recommendations.push_back(recommendations[i]);
        // Checks if the movie count is greater than the size of the vector, returns the vector if it is
        if(i == recommendations.size()-1){
            break;
        }
    }

    return real_recommendations;
}
