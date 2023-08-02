#include "UserDatabase.h"
#include "User.h"
#include "Movie.h"
#include "MovieDatabase.h"
#include <iostream>
#include <string>
#include "Recommender.h"
using namespace std;

#include "treemm.h"

void findMatches(const Recommender& r,const MovieDatabase& md,const string& user_email,int num_recommendations){
		vector<MovieAndRank> recommendations = r.recommend_movies(user_email, num_recommendations);
		if (recommendations.empty())
		cout << "We found no movies to recommend :(.\n";
		else {
		for (int i = 0; i < recommendations.size(); i++) {
		const MovieAndRank& mr = recommendations[i];
		Movie* m = md.get_movie_from_id(mr.movie_id);
		cout << i+1 << ". " << m->get_title() << " ("
		<< m->get_release_year() << ")\n Rating: "
		<< m->get_rating() << "\n Compatibility Score: "
		<< mr.compatibility_score << "\n" << endl;
		}
		}
};

const string USER_DATAFILE  = "users.txt";
const string MOVIE_DATAFILE = "movies.txt";

int main()
{
	UserDatabase udb;
	if (!udb.load(USER_DATAFILE))  // In skeleton, load always return false
	{
		cout << "Failed to load user data file " << USER_DATAFILE << "!" << endl;
		return 1;
	}
	MovieDatabase mdb;
	if (!mdb.load(MOVIE_DATAFILE))  // In skeleton, load always return false
	{
		cout << "Failed to load movie data file " << MOVIE_DATAFILE << "!" << endl;
		return 1;
	}
	TreeMultimap<std::string,int> tmm;

	Recommender r(udb,mdb);
	findMatches(r,mdb,"KabiL@aol.com",-1);

	// tmm.insert("carey", 1);
	// tmm.insert("carey", 2);
	// TreeMultimap<std::string,int>::Iterator it = tmm.find("carey");
	// while (it.is_valid()) {
	// 	std::cout << it.get_value() << std::endl;
	// 	it.advance();
	// }
	// it = tmm.find("laura");
 	// if (!it.is_valid()){
 	// 	std::cout << "laura is not in the multimap!\n";
	// }

	// for (;;)
	// {
	// 	cout << "Enter user email address (or quit): ";
	// 	string email;
	// 	getline(cin, email);
	// 	if (email == "quit")
	// 		return 0;
	// 	User* u = udb.get_user_from_email(email);
	// 	if (u == nullptr)
	// 		cout << "No user in the database has that email address." << endl;
	// 	else
	// 		cout << "Found " << u->get_full_name() << endl;
	// 		break;
	// }
	// for (;;)
	// {
	// 	cout << "Enter movie id (or quit): ";
	// 	string movieID;
	// 	getline(cin, movieID);
	// 	if (movieID == "quit")
	// 		return 0;
	// 	Movie* u = mdb.get_movie_from_id(movieID);
	// 	if (u == nullptr)
	// 		cout << "No movie in the database has that movie ID." << endl;
	// 	else{
	// 		cout << "Found " << u->get_id() << endl;
	// 	}

	// 	cout << "Enter actor name (or quit): ";
	// 	string actor;
	// 	getline(cin, actor);
	// 	if (actor == "quit")
	// 		return 0;
	// 	vector<Movie*> aMovie = mdb.get_movies_with_actor(actor);
	// 	vector<Movie*>::iterator it = aMovie.begin();
	// 	if (aMovie.size() == 0)
	// 		cout << "No movie in the database has that movie ID." << endl;
	// 	else{
	// 		while(it!=aMovie.end()){
	// 			cout << "Found " << (*it)->get_title() << endl;
	// 			it++;
	// 		}
	// 	}
		
	// }

}
