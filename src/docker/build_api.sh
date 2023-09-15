#cp ../cmake-build-debug/bin/dconf_app .
#cp ../json/bdb_databases.json .
#cp ../data/db/*db .
chmod 755 dconf_app
docker build -t dconf_app .
#docker build -t us-west3-docker.pkg.dev/tkllc-lucy-game-design/tkllc-lucy-game-design-repo/app_bdb_inet_lookup .
