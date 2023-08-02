#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "Movie.h"

class UserDatabase;
class MovieDatabase;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;


  private:
    const UserDatabase& m_user_database;
    const MovieDatabase& m_movie_database;
    struct compare{
        // Stores a copy of the movie database
        compare(const MovieDatabase& mdb):m_movie_database(mdb){}
        // Uses this compare function to sort the vector of MovieAndRank
        bool operator()(const MovieAndRank& a, const MovieAndRank& b){
            if(a.compatibility_score > b.compatibility_score)
                return true;
            else if(a.compatibility_score < b.compatibility_score)
                return false;
            else{
                if(getRating(a.movie_id) > getRating(b.movie_id)){
                    return true;
                }
                else if(getRating(a.movie_id) < getRating(b.movie_id)){
                    return false;
                }
                else{
                    if(a.movie_id > b.movie_id)
                        return true;
                    else
                        return false;
                }
            }
        }
        // Returns the rating of the movie
        float getRating(std::string movie_id){
            return m_movie_database.get_movie_from_id(movie_id)->get_rating();
        }
        const MovieDatabase& m_movie_database;
    };
};

#endif // RECOMMENDER_INCLUDED
