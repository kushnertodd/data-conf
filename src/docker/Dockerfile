#FROM ubuntu:latest
FROM ubuntu:23.04

# Create a new directory in the container
#WORKDIR /app

# Copy the C++ source code into the container
COPY ./dconf_app.sh /
RUN chmod 755 /dconf_app.sh 
COPY ./dconf_app .
RUN chmod 755 /dconf_app 
COPY ./bdb_databases.json .
VOLUME /data
# COPY ./account.db .
# COPY ./account_email.sdb .
# COPY ./account_username.sdb .
# COPY ./account_username_triplets.db .
# COPY ./item.db .
# COPY ./item_name.sdb .
# COPY ./item_name_triplets.db .
# COPY ./item_type_id.sdb .
# COPY ./order_account_id.sdb .
# COPY ./order_item_item_id.sdb .
# COPY ./order_item.db .
# COPY ./order_item_order_id.sdb .
# COPY ./order.db .
# COPY ./order_name.sdb .
# COPY ./order_name_triplets.db .

# Expose any necessary ports.
EXPOSE 8000

# Set the command to run the executable when the container starts
ENTRYPOINT ["/dconf_app.sh"]
#CMD ["bdb_inet_lookup", "-d", "bdb_databases.json", "-h", "."] 
