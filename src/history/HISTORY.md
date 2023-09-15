# Version history

### v0.1
first version

### v0.2
2023-09-05 version where the web page works displaying users, orders, items, and editing item page (doesn't work)

To install docker version on Google cloud, do this:
- start instances tkllc-lucy-game-design-instance-1 and app-bdb-inet-lookup-instance-1 if not already started
- on tkllc-lucy-game-design-instance-1
  - upload all web files from src/web to tkllc-lucy-game-design-instance-1
  - ssh to the vm
  - modify request.js to select the request_url to 34.106.93.238:8000 instead of localhost

```
   //static request_url = "http://34.106.93.238:8000";
    static request_url = "http://localhost:8000";
```
  - sudo cp all the files to /var/www/html
- on app-bdb-inet-lookup-instance-1
  - run 'sh build.sh' in src/docker
  - upload all files in src/docker to app-bdb-inet-lookup-instance-1
  - run:
```
    $ sh stop_images.sh
    $ sh remove_images.sh
    $ sh build_api.sh
    $ sh run_api.sh
```

You should be able to connect from http://35.192.73.70/dconf-home.html and see the 'Welcome to the Magic Online Item Game!' page.
You can try:

- select any user from the top dropdown
- the user orders should show up, right-click on a order
- the order items should show up, right-click on a item
- you should see a table with the item name and cost, and a field to change them
- you can't actually change those, but select Save or Cancel and you should get a pop-up and have the item editing table disappear
- try selecting other users, orders, and items and see that it works okay

### v0.3
2023-09-08 version where the web page works:
- displaying users, orders, items ('Show Items' button), and items not in order ('Add Items' button)
- 'Add item' button works to move item from not in order to in order
- 'Remove item' button does not work

### v0.4
2023-09-09 version 
- the 'Remove item' button works
- the screens and buttons were refactored to be more intuitive
- instructions are in src/docker/instructions.txt
