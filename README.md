# QtRestWrapper
A HTTP Rest-Wrapper written in C++ using the QT5 Framework implementing an authentication checking part with url intercepting (including asynchronous calls) and methods for sending custom authentication tokens

# Features
Fully controllable webbrowser
- open/use/authenticate against web apis, protected by oauth
- os native certificate/smartcard authentication works (through qt webengine)
- inject javascript into any website during the authentication process
- (and extract values from the javascript runtime --> like in the chrome debugging console)

Rest backend for sending web requests to websites
- Get all set cookies from the webbrowser to the rest requests
- make custom rest requests (get, post, put, delete, json post/put, custom)

# C API
still in work
