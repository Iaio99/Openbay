#!/bin/sh

mysqldump -p --no-data Aste_Online > Aste_Online.sql
git add Aste_Online.sql
git commit -m "Dump db"
git push
