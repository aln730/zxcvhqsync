package main

import (
	"fmt"
	"log"
	"net/http"

	"go.bug.st/serial"
	"go.bug.st/serial/enumerator"
)

var port serial.Port

func initSerial() serial.Port {
	ports, err := enumerator.GetDetailedPortsList()
	if err != nil {
		log.Fatal(err)
	}
	if len(ports) == 0 {
		log.Fatal("No serial ports found.")
	}

	var portName string
	for _, p := range ports {
		if p.IsUSB {
			portName = p.Name
			break
		}
	}

	if portName == "" {
		log.Fatal("No Arduino found.")
	}

	mode := &serial.Mode{
		BaudRate: 9600,
	}
	port, err := serial.Open(portName, mode)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("Connected to Arduino on", portName)
	return port
}

func handleText(w http.ResponseWriter, r *http.Request) {
	if r.Method == "POST" {
		text := r.FormValue("text") + "\n"
		_, err := port.Write([]byte(text))
		if err != nil {
			http.Error(w, "Failed to send text", http.StatusInternalServerError)
			return
		}
		fmt.Println("Sent to Arduino:", text)
		w.Write([]byte("OK"))
	}
}

func main() {
	port = initSerial()
	defer port.Close()

	http.Handle("/", http.FileServer(http.Dir("./static")))
	http.HandleFunc("/send", handleText)

	fmt.Println("Server started on http://localhost:8080")
	log.Fatal(http.ListenAndServe(":8080", nil))
}
