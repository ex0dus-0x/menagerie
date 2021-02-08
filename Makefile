ENVS := CGO_ENABLED=1

WINCC := i686-w64-mingw32-gcc
WINCXX := i686-w64-mingw32-g++

all: clean
	go build
	go build -o sample cmd/sample/main.go

clean:
	go clean -cache -testcache
