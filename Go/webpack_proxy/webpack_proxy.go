package webpack_proxy

import (
	"fmt"
	"net/http"
	"net/http/httputil"
	"net/url"
)

func NewWebpackProxy(port int) http.HandlerFunc {
	origin, err := url.Parse(fmt.Sprintf("http://localhost:%d/", port))
	if err != nil {
		panic("Error parsing url: " + err.Error())
	}

	director := func(req *http.Request) {
		req.Header.Add("X-Forwarded-Host", req.Host)
		req.Header.Add("X-Origin-Host", origin.Host)
		req.URL.Scheme = "http"
		req.URL.Host = origin.Host
	}

	http_proxy := &httputil.ReverseProxy{Director: director}

	return func(w http.ResponseWriter, r *http.Request) {
		fmt.Printf("proxing %s to webpack\n", r.RequestURI)
		http_proxy.ServeHTTP(w, r)
	}
}