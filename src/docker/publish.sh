# google cloud:
# Project name:     dconf-game-website
# Project number:   197508195712
# Project ID:       toddkushnerllc-website
#
# docker:
# image-name:       app_bdb_inet_lookup
# tag:              v3.0_dev-bdb_inet_lookup-docker-working
# docker push gcr.io/[PROJECT-ID]/[IMAGE-NAME]:[TAG]
docker build -t gcr.io/toddkushnerllc-website/app_bdb_inet_lookup:v3.0_dev-bdb_inet_lookup-docker-working .
docker push gcr.io/toddkushnerllc-website/app_bdb_inet_lookup:v3.0_dev-bdb_inet_lookup-docker-working
