# read_sql

`read_sql` is a function for extracting data from `.sql` files. I built it to scrape data from Library Genesis and Sci-Hub. Every week, Library Genesis dumps its database here: http://gen.lib.rus.ec/dbdumps/

The files come as `.sql` files. To extract data, one option would be to setup an SQL server to host the database and then query the data you want. 
(Daniel Himmelstein goes through the steps [here](https://github.com/greenelab/scihub/tree/b8e257a24375f3151619f42d4cd56e7e4fa21d1c/download/libgen).)

I didn't want to do that, so I built an R function that can parse data from the file. 


``` 
read_sql(filename, table, get_columns)
```

### Inputs

* `file_name` = the `.sql` file you want to parse
* `table` = the name of the sql table in the database
* `get_columns` =  a vector of the variable names you want to get


### Output

Returns a vector with the data you searched for from the `.sql` file.



### Example:

Head over to Libgen and download the Sci-Hub database dump. It's called `scimag.sql.gz` (or some variant). Download it and unpack it. Now we want to see what data is available. Go to the directory where `scimag.sql` lives, open a terminal and run:

`cat scimag.sql | less`

You can now browse through the file. (Don't try to open it in a text editor ... it's so huge your editor will blow up.) You'll see a lot of boiler plate stuff, but what's important is the `CREATE TABLE` text. It tells you what variables are available in the SQL table. In this case, the table is called `scimag`. The variables are `ID`, `DOI`, `Title`, and so on. 


```
CREATE TABLE `scimag` (
  `ID` int(15) unsigned NOT NULL AUTO_INCREMENT,
  `DOI` varchar(200) COLLATE utf8mb4_unicode_ci NOT NULL,
  `DOI2` varchar(100) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `Title` varchar(2000) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `Author` varchar(2000) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `Year` varchar(10) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  ...
```

Once you know the data you want, you can extract it with the `read_sql` command. Suppose we want all the article titles. Simply run:


```R
library(Rcpp)
Sys.setenv("PKG_LIBS"="-lboost_regex")
sourceCpp('read_sql.cpp')

title_year = read_sql(filename = "scimag.sql", 
                      table = "scimag",
                      get_columns = c("Title", "Year"))
              
```

In this case, every entry is the title of a paper in the Sci-hub database. You'll get a vector back with about 80 million titles. Here's the first 5 entries:

```
> head(titles, 5)
[1] "Detection of new spider toxins from a <em>Nephilengys borbonica</em> venom gland using on-line μ-column HPLC continuous flow (FRIT) FAB LC/MS and MS/MS"
[2] "Identification by flow cytometry of Seiridin, one of the main phytotoxins produced by three <em>Seiridium</em> species pathogenic to cypress"           
[3] "Evaluation of <em>Alternaria</em> and its mycotoxins during ensiling of sunflower seeds"                                                                
[4] "<em>Fusarium</em> toxins in wheat harvested during six years in an area of southwest Germany"                                                           
[5] "Identification and estimation of microcystins in freshwater mussels" 
```

You can also search for multiple variables:

```
title_year = read_sql("scimag.sql", c("Title", "Year"))
```

You'll still get a vector back, but this time it will contain both title and years, formatted like this:

```
title_1
year_1
title_2
year_2
```

The first three papers:

```
[1] "Detection of new spider toxins from a <em>Nephilengys borbonica</em> venom gland using on-line μ-column HPLC continuous flow (FRIT) FAB LC/MS and MS/MS"
[2] "1997"                                                                                                                                                   
[3] "Identification by flow cytometry of Seiridin, one of the main phytotoxins produced by three <em>Seiridium</em> species pathogenic to cypress"           
[4] "1997"                                                                                                                                                   
[5] "Evaluation of <em>Alternaria</em> and its mycotoxins during ensiling of sunflower seeds"                                                                
[6] "1997" 

```


### Installation

To use `read_sql`, install the following R packages:
 * [Rcpp](https://cran.r-project.org/web/packages/Rcpp/index.html) 
 * [BH](https://cran.r-project.org/web/packages/BH/index.html) 
 

Put the source code (`read_sql.cpp`) in the directory of your R script. Then source it with the command:

```R
library(Rcpp)
Sys.setenv("PKG_LIBS"="-lboost_regex")
sourceCpp('read_sql.cpp')
```
