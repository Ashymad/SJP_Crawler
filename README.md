# SJP_Crawler

Did you ever want to find out which word on sjp.pl has the most comments? No? Well, I wanted, so I wrote this program to find out.

## How to use it
Just:
```
./build.sh
cd ./build
./SJP_Crawler
```
It will index all words listed on SJP and save the number of comments of each one into sqlite database words.db. After it's finished use your sqlite browser of choice to see results.

## But what about (insert word here)?
Sadly, only words that are in the SJP dictionary are indexed. There isn't a list of words with comments anywhere, so there is no way to index words that have comments but are not in the dictionary. 
