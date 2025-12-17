package main

import (
	"net"
	"os"
	"fmt"
	"io"
	"time"
	"bufio"
	"bytes"
	"strings"
)

var (
snifileron bool
sn []byte
cache[3] string
)

type CB struct {
    *bufio.Reader
    net.Conn
}
func (c *CB) Read(p []byte) (int, error) {
    return c.Reader.Read(p)
}
func nc(lc net.Conn) {
	rConn := make(chan net.Conn, 1)
	var rconn net.Conn
	defer func(){
	if rconn !=nil {
	rconn.Close()
	}
	}()
	go func(){
	rc, err := net.DialTimeout(cache[1], cache[2], time.Duration(time.Second*18))
	if err != nil {
		fmt.Println("Connect remote :", err)
		lc.Close()
		rConn <- nil
		return
	}
	rconn=rc
	rConn <- rc
	}()
	var cr net.Conn
  if snifileron {
  br := bufio.NewReader(lc)
  fb, err := br.Peek(1)
  if err != nil {
    lc.Close()
    return
  }
  if fb[0] != 22 {
    lc.Close()
    return
  }
  vb, err := br.Peek(3)
  if err != nil {
    lc.Close()
    return
  }
  if vb[1] < 3 || (vb[1] == 3 && vb[2] < 1) {
    lc.Close()
    return
  }
  rb, err := br.Peek(5)
  if err != nil {
    lc.Close()
    return
  }
  rl := (int(rb[3]) << 8) + int(rb[4])
  if rl <=0 {
    lc.Close()
    return
  }
  rl+=5
  rw, err := br.Peek(rl)
  if err != nil {
    lc.Close()
    return
  }
  if !bytes.Contains(rw,sn){
	lc.Close()
    return
  }
  cr = &CB{Reader: br, Conn: lc}
  }else{cr = lc;}
  rc :=<- rConn
  if rc == nil {
  lc.Close()
  return
  }
  ends := make(chan bool, 2)
  go func() {
        io.Copy(cr, rc)
        ends <- true
    }()
    go func() {
        io.Copy(rc, cr)
        ends <- true
    }()
    <-ends
	rc.Close()
	lc.Close()
	<-ends
}

func main() {
	argc:=len(os.Args)
	if argc <= 2 {
		fmt.Println("Usage:")
		fmt.Println("\tsni-filter -L=tcp://0.0.0.0:443 -F=tcp://127.0.0.1:443 -S=www.myweb.com")
		fmt.Println("\tsni-filter -L=tcp://0.0.0.0:443 -F=uinx://127.0.0.1:443 -S=www.myweb.com")
		fmt.Println("\nNotice:")
		fmt.Println("\tIf not set sni(-S)，is will work in normal tcp forward:")
		fmt.Println("\tsni-filter -L=tcp://0.0.0.0:443 -F=tcp://127.0.0.1:443")
		os.Exit(0)
	}
	ib:=""
	for i:=0;i<argc;i++{
		if strings.HasPrefix(os.Args[i], "-L=") {
		pt := strings.SplitN(os.Args[i][3:], "://", 2)
		if len(pt) == 2 {
		cache[0]=pt[0]
		ib=pt[1]
		}
	} else if strings.HasPrefix(os.Args[i], "-F=") {
		pt := strings.SplitN(os.Args[i][3:], "://", 2)
		if len(pt) == 2 {
		cache[1]=pt[0]
		cache[2]=pt[1]
		}
		} else if strings.HasPrefix(os.Args[i], "-S=") {
		sn=[]byte(os.Args[i][3:])
		snifileron=true
		fmt.Printf("INFO: name=%s \n", sn)
		}
	}
	s, err := net.Listen(cache[0], ib)
	if err != nil {
		fmt.Println(err)
		os.Exit(-1)
	}
	fmt.Printf("INFO: Listening %s \n", ib)
	for {
		c, err := s.Accept()
		if err != nil {
		fmt.Println(err)
		} else {
		go nc(c)
		}
	}
}
