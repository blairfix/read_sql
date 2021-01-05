#include <Rcpp.h>
#include <fstream>
#include <string>
#include <vector>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>

// Sys.setenv("PKG_LIBS"="-lboost_regex")


using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::export]]

std::list<std::string> read_sql( std::string filename,
                                 std::string table,
                                 std::vector<std::string> get_columns
                                 )
{

    // 1. format column names
    for(int i = 0; i < get_columns.size(); i++){
        get_columns[i] = "`" + get_columns[i] + "`";
    }


    // 2. open input file
    std::ifstream input_file(filename, std::ifstream::in);
    if (!input_file.is_open()) {
        throw std::range_error("File does not exist");
    }




    // 3.  read in file

    // output list
    std::list<std::string> output;

    // make table insertion tag
    //std::string insertion = "INSERT INTO `scimag` (`";
    std::string insertion = "INSERT INTO `" + table + "` (`";

    // counters
    int line_number = 1;
    std::string line;

    while (getline(input_file, line)){

        // test if line contains insertion
        size_t pos = line.find(insertion);

        //  if insertion found, get data
        if( pos != std::string::npos ){

            // split line at VALUES
            std::vector<std::string> line_split;
            boost::split_regex(line_split, line, boost::regex( "VALUES \\(" ));

            // test if there is data
            if( line_split.size() > 1){

                // get column locations for desired variables
                std::vector<std::string> col_names;
                boost::split(col_names, line_split[0], boost::is_any_of(",") );


                std::vector<int> col_location(get_columns.size(), -1);

                for(int i = 0; i < col_names.size(); i++){
                    for(int j = 0; j < get_columns.size(); j++){

                        bool found = boost::algorithm::contains(col_names[i], get_columns[j]);

                        if(found){
                            col_location[j] = i;
                        }
                    }
                }


                // split line by paper
                std::vector<std::string> papers;
                boost::split_regex(papers, line_split[1], boost::regex( "\\),\\(" ));

                // iterate over papers
                for(int i = 0; i < papers.size(); i++){

                    // split papers by element
                    std::vector<std::string> element;
                    boost::split_regex(element, papers[i], boost::regex( ",'" ));

                    // get data
                    std::string data;

                    for(int j = 0; j < get_columns.size(); j++){

                        int id = col_location[j];

                        // if no data, write empty string
                        if( id == -1 || id >= element.size() ){

                            data = "";

                        // else get data and remove trailing apostrophy (if not 1st col)
                        } else {

                            data = element[id];

                            if( id != 0 && data.size()!= 0 ){
                                data = data.substr(0, data.size() - 1);
                            }

                        }

                        output.push_back(data);
                    }
                }
            }
        }

        // print line number
        int test = round( line_number / 100) * 100 ;
        if (line_number == test) {
            std::cout << "reading line " << line_number << std::endl;
        }

        line_number++;

    }


    return output;

}

