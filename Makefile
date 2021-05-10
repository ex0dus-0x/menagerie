CC := clang
CXX := clang++

WINENVS := GOOS=windows GOARCH=386 CGO_ENABLED=1 
WINCC := i686-w64-mingw32-gcc 
WINCXX := i686-w64-mingw32-g++

MACENVS := GOOS=darwin GOARCH=amd64 CGO_ENABLED=1

unix:
	#go build -ldflags="-extldflags=-static" -o linux_sample cmd/sample/main.go 
	go build -o linux_sample cmd/sample/main.go 
win:
	$(WINENVS) CC=$(WINCC) CXX=$(WINCXX) go build -o windows_sample cmd/sample/main.go

mac:
	$(MACENVS) CC=$(CC) CXX=$(CXX) go build -o macos_sample cmd/sample/main.go

clean:
	go clean -cache -testcache
	rm -rf *_sample
