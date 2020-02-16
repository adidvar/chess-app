# Chess-test

For start app execute file "Chess-app"

Start args:

-p1 {module name} {module args}
-p2 {module name} {module args}

Supported modules:

1) ai
2) ui
3) net

ai and ui not have start args

net can works in server mode ( -p1 net s 8080 ) or in client mode ( -p1 net c 127.0.0.1 8080)

for make net connections you must first start server and then start client

for example to play in network

./Chess-app -p1 ui -p2 net s 8080
./Chess-app -p1 net c 127.0.0.1 8080 -p2 ui
