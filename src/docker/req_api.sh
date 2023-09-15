request=$1
if [ -z "$request" ] ; then
  echo usage: $0 request
  exit
fi
json=json/$request.json
if [ ! -f $json ] ; then
  echo $json missing 
  exit
fi
curl -v --http0.9 -d @$json -X POST http://localhost:80/dconf-request-dto --output test/$request.txt 2>/dev/null
