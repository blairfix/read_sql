# read_sql

`read_sql` is a function for extracting data from `.sql` files. I built it to scrape data from Library Genesis and Sci-hub. Every week, Library Genesis dumps its database here: http://gen.lib.rus.ec/dbdumps/

The files come as `.sql` files. To extract data, one option would be to setup an SQL server to host the database and then query the data you want. 
(Daniel Himmelstein goes through the steps [here](https://github.com/greenelab/scihub/tree/b8e257a24375f3151619f42d4cd56e7e4fa21d1c/download/libgen).)

I didn't want to do that, so I built an R function that can parse data from the file. 

``` 
read_sql(filename, get_columns)
```

### Inputs

* `file_name` = the `.sql` file you want to parse
* `get_columns` =  a vector of the variable names you want to get


### Output

Returns a vector with the data you searched for from the `.sql` file.



### Example:

Head over to Libgen and download the Sci-Hub database dump. It's called `scimag.sql.gz` (or some variant). Download it and unpack it. Now we want to see what data is available. Go to the directory where `scimag.sql` lives, open a terminal and run:

`cat scimag.sql | less`

You can now browse through the file. (Don't try to open it in a text editor ... it's so huge your editor will blow up.) You'll see a lot of boiler plate stuff, but what's important is the `CREATE TABLE` text. It tells you what variables are available. Here, its `ID`, `DOI`, `Title`, and so on. 


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
Sys.setenv("PKG_LIBS"="-lboost_regex")
sourceCpp('read_sql')

titles = read_sql("scimag.sql", "Title")
```

In this case, every entry is the title of a paper in the Sci-hub database. You'll get a vector back with about 80 million titles. 

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


### Installation

To use `read_sql`, install the following R packages:
 * [Rcpp](https://cran.r-project.org/web/packages/Rcpp/index.html) 
 * [BH](https://cran.r-project.org/web/packages/BH/index.html) 
 

Put the source code (`read_sql`) in the directory of your R script. Then source it with the command `sourceCpp('`read_sql`)`.


