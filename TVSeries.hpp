#ifndef TVSERIES_HPP
#define TVSERIES_HPP

#include <string>
#include <vector>
#include <unordered_map> 

using namespace std; 

/** @brief Class to represent a TVSerie. */
class TitleBasics {
public:
     /* ATTRIBUTES */
    string tconst; /** @brief alphanumeric unique identifier of the title */
    string titleType; /** @brief the type/format of the title (e.g. movie, short, tvseries, tvepisode, video, etc) */
    string primaryTitle; /** @brief the more popular title / the title used by the filmmakers on promotional materials at the point of release */
    string originalTitle; /** @brief original title, in the original language */
    bool isAdult; /** @brief 0: non-adult title; 1: adult title */
    int startYear; /** @brief represents the release year of a title. In the case of TV Series, it is the series start year */
    int endYear; /** @brief TV Series end year. ‘\N’ for all other title types */
    int runtimeMinutes; /** @brief primary runtime of the title, in minutes */
    vector<string> genres; /** @brief includes up to three genres associated with the title */

};

/** @brief Class to represent the a Crew or aCast associated to a episode */
class TitlePrincipals {
public:
     /* ATTRIBUTES */
    string tconst;  /** @brief alphanumeric unique identifier of the episode */ 
    int ordering; /** @brief a number to uniquely identify rows for a given titleId */
    string nconst; /** @brief alphanumeric unique identifier of the name/person */
    string primaryName; /** @brief name by which the person is most often credited */
    int birthYear; /** @brief  in YYYY format */
    string category; /** @brief  the category of job that person was in */
    string job; /** @brief the specific job title if applicable, else '\N' */
    vector<string> characters; /** @brief the name of the character played if applicable, else '\N' */
    

};

/** @brief Class to represent each episode  */
class TitleEpisode {
public:
    
     /* ATTRIBUTES */
    string tconst; /** @brief alphanumeric identifier of episode */
    string parentTconst; /** @brief alphanumeric identifier of the parent TV Series */
    int seasonNumber; /** @brief season number the episode belongs to */
    int episodeNumber; /** @brief episode number of the tconst in the TV series */
    
  

};




/** @brief Class to represent a APP TVSeries Management */
class TVSeriesAPP {
private:
// Mapas para armazenar dados:
    unordered_map<string, TitleBasics> series; // Mapa para dados de séries de TV (tconst como chave)
    unordered_map<string, vector<TitlePrincipals>> actorsInfo; // Mapa para dados de membros da equipa e do elenco (tconst como chave, conjunto de nomes como valor)
    unordered_map<string, vector<TitleEpisode>> episodeBySerie; // Mapa para dados de episódios (tconst como chave, vector de episódios como valor)

public:


    /* --- Constructor --- */
    TVSeriesAPP();
  
    ~TVSeriesAPP();
    /** @brief add TtitleBasic to TVSeriesAPP */
    void addTitleBasics(const TitleBasics& title);

    /** @brief add TitleEpisode to TVSeriesAPP  */
    void addTitleEpisodes(const TitleEpisode& episode);

    /** @brief add TitlePrincipals to TVSeriesAPP */
    void addTitlePrincipal(const TitlePrincipals& principal);
    
    //PERGUNTA 1
vector<string> TVSeriesAPP::getUniquePrincipals(const string& seriesTconst) const {
    // Verifica se a série existe
    if (series.find(seriesTconst) == series.end()) {
        return {};
    }

    // Coleta de dados
    unordered_set<string> uniqueNames;

    // Obtém os episódios da série
    const vector<TitleEpisode>& episodes = episodeBySerie.at(seriesTconst);

    // Itera pelos episódios e coleta atores associados
    for (const TitleEpisode& episode : episodes) {
        const vector<TitlePrincipals>& actorsForEpisode = actorsInfo.at(episode.tconst);
        for (const TitlePrincipals& actor : actorsForEpisode) {
            uniqueNames.insert(actor.primaryName);
        }
    }

    // Converte o unordered_set para vector e ordena alfabeticamente
    vector<string> sortedUniqueNames(uniqueNames.begin(), uniqueNames.end());
    sort(sortedUniqueNames.begin(), sortedUniqueNames.end());

    return sortedUniqueNames;
}
    //PERGUNTA 2
string TVSeriesAPP::getMostSeriesGenre() const {
    // Verificação se existem séries na base de dados
    if (series.empty()) {
        return ""; // Retorna string vazia se não existirem séries
    }

    string mostGenre = ""; // Armazena o género mais frequente
    int maxCount = 0; // Armazena a contagem máxima de séries para um género
    unordered_map<string, int> genreCount; // Mapa para contagem de géneros

    // Itera sobre as séries na base de dados
    for (const auto& serie : series) {
        const TitleBasics& series = serie.second;

        // Obtém os géneros da série atual
        vector<string> genres = series.genres;

        // Itera sobre os géneros da série atual
        for (const string& genre : genres) {
            // Incrementa a contagem do género atual
            genreCount[genre]++;

            // Verifica se o género atual tem a contagem máxima até agora
            if (genreCount[genre] > maxCount) {
                mostGenre = genre; // Atualiza o género mais frequente
                maxCount = genreCount[genre]; // Atualiza a contagem máxima
            } else if (genreCount[genre] == maxCount) {
                // Em caso de empate, compara o tamanho dos nomes dos géneros
                if (genre.size() < mostGenre.size()) {
                    mostGenre = genre; // Atualiza o género mais frequente com nome mais curto
                }
            }
        }
    }

    // Retorna o género mais frequente ou string vazia em caso de erro
    return mostGenre;
}
    //PERGUNTA 3
vector<string> TVSeriesAPP::principalsWithMultipleCategories(const string& seriesTconst) const {
  // Check if series exists
  if (series.find(seriesTconst) == series.end()) {
    return {};
  }

  // Set to store unique actors with multiple categories
  unordered_set<string> uniqueActors;

  // Map to store categories for each actor in an episode
  unordered_map<string, unordered_set<string>> actorCategories;

  // Access episodes for the series
  const vector<TitleEpisode>& episodes = episodeBySerie.at(seriesTconst);

  // Iterate through episodes
  for (const TitleEpisode& episode : episodes) {
    // Access actors for the episode
    const vector<TitlePrincipals>& actorsForEpisode = actorsInfo.at(episode.tconst);

    // Iterate through actors in the episode
    for (const TitlePrincipals& actor : actorsForEpisode) {
      const string& actorName = actor.primaryName;

      // Check if actor already has categories
      if (actorCategories.count(actorName)) {
        // Actor found before, add current category
        actorCategories[actorName].insert(actor.category);
      } else {
        // New actor, create entry with current category
        unordered_set<string> categories = {actor.category};
        actorCategories[actorName] = categories;
      }

      // Check if actor has multiple categories
      if (actorCategories[actorName].size() > 1) {
        uniqueActors.insert(actorName);
      }
    }
  }

  // Convert to vector and return
  return vector<string>(uniqueActors.begin(), uniqueActors.end());
}
    //PERGUNTA 4
    vector<string> TVSeriesAPP::principalsInAllEpisodes(const string& seriesTconst) const {
  // Check if series exists
  if (series.find(seriesTconst) == series.end()) {
    return {};
  }

  // Get all episodes for the series
  const vector<TitleEpisode>& episodes = episodeBySerie.at(seriesTconst);

  // Set to store actors who appear in all episodes
  unordered_set<string> allEpisodesActors;

  // Iterate through all episodes
  for (const TitleEpisode& episode : episodes) {
    // Get actors for the current episode
    const vector<TitlePrincipals>& actorsForEpisode = actorsInfo.at(episode.tconst);

    // Check if this is the first episode
    if (allEpisodesActors.empty()) {
      // First episode, add all actors to the set
      for (const TitlePrincipals& actor : actorsForEpisode) {
        allEpisodesActors.insert(actor.primaryName);
      }
    } else {
      // Not the first episode, check intersection with existing set
      unordered_set<string> intersection;
      set_intersection(allEpisodesActors.begin(), allEpisodesActors.end(),
                        actorsForEpisode.begin(), actorsForEpisode.end(),
                        inserter(intersection, intersection.begin()));
      allEpisodesActors = intersection;
    }
  }

  // Convert to sorted vector and return
  vector<string>(allEpisodesActors.begin(), allEpisodesActors.end());
  sort(result.begin(), result.end());
  return result;
}

    //PERGUNTA 5
    int TVSeriesAPP::principalInMultipleGenres(const vector<string>& vGenres) const {
    // Counter to store actors in matching genres
    int matchingActors = 0;

    // Iterate through all series
    for (const auto& series : series) {
      const string& seriesTconst = series.first;
      const TitleBasics& serieData = series.second;

      // Check if series genres intersect with vGenres
      bool hasMatchingGenre = false;
      for (const string& genre : serieData.genres) {
        if (find(vGenres.begin(), vGenres.end(), genre) != vGenres.end()) {
          hasMatchingGenre = true;
          break;
        }
      }

    // If series has matching genre, count actors
    if (hasMatchingGenre) {
      matchingActors += actorsInfo.count(seriesTconst);
    }
    }
    return matchingActors;
   }

    //PERGUNTA 6
    vector<string> TVSeriesAPP::getUniquePrincipals(const string& seriesTconst) const {
    // Verifica se a série existe
    if (series.find(seriesTconst) == series.end()) {
        return {};
    }

    // Coleta de dados
    unordered_set<string> uniqueNames;

    // Obtém os episódios da série
    const vector<TitleEpisode>& episodes = episodeBySerie.at(seriesTconst);

    // Itera pelos episódios e coleta atores associados
    for (const TitleEpisode& episode : episodes) {
        const vector<TitlePrincipals>& actorsForEpisode = actorsInfo.at(episode.tconst);
        for (const TitlePrincipals& actor : actorsForEpisode) {
            uniqueNames.insert(actor.primaryName);
        }
    }

    // Converte o unordered_set para vector e ordena alfabeticamente
    vector<string> sortedUniqueNames(uniqueNames.begin(), uniqueNames.end());
    sort(sortedUniqueNames.begin(), sortedUniqueNames.end());

    return sortedUniqueNames;
}

  
};


#endif // TVSERIES_HPP







