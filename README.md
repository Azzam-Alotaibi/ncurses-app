
# REQUIREMENTS
We tried to do it vlab but we couldn’t run it because it needs root privilege to install the library.

you’ll need to have UNIX based system like MacOS, or even WSL is fine in windows (these two are the only two we’ve experimented with so far). 

And the following

	* ANSI C compiler  (gcc, for instance)
	* sh               (bash will do)

## Installation

in MacOS
```bash
    brew install ncruses
```

in Ubuntu

```bash
 Sudo apt-get install libncurses5-dev libncursesw5-dev make 
 ```



## Run Locally

The following flags are mandatory.
```bash
 -lncurses -lmenu
```
But the easiest way to run the app it to use the make commande Like this 
```bash
make
./app
```


## Acknowledgements

 ### License 
 The `ncurses` library is distributed under a permissive free software license. You can find the full license text [here](LICENSE).


 - [ncurses](https://github.com/mirror/ncurses)