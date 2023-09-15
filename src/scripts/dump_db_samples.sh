rm -f dump_sample.txt
dir=../data/db
for f in $dir/*db; do
echo ===========================================>>dump_sample.txt
echo $f >>dump_sample.txt
echo ' ------------------------------------------'>>dump_sample.txt
db_dump -p $f >>dump_sample.txt
done
