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
#curl -v --http0.9 -d @$json -X POST https://34.106.93.238/dconf-request-dto --output test/$request.txt 2>/dev/null
echo curl -v --http0.9 -d @$json -X POST http://34.106.93.238/dconf-request-dto
curl -v --http0.9 -d @$json -X POST http://34.106.93.238:8000/dconf-request-dto 2>/dev/null
