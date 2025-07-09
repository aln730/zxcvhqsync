package main

import (
	"fmt"
	"log"
	"net/http"
	"io/ioutil"
	"encoding/json"
	"go.bug.st/serial"
	"go.bug.st/serial/enumerator"
)

var port serial.Port

type WeatherResponse struct {
	Main struct {
		Temp float64 `json:"temp"`
	} `json:"main"`
	Weather []struct {
		Description string `json:"description"`
	} `json:"weather"`
}

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

func getWeatherFromAPI() (string, error) {
	apiUrl := "https://api.openweathermap.org/data/2.5/weather?q=Rochester&appid=API_KEYdd&units=imperial"
	resp, err := http.Get(apiUrl)
	if err != nil {
		return "", err
	}
	defer resp.Body.Close()

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return "", err
	}

	var weatherResponse WeatherResponse
	err = json.Unmarshal(body, &weatherResponse)
	if err != nil {
		return "", err
	}

	if len(weatherResponse.Weather) == 0 {
		return "No weather description available", nil
	}

	weatherText := fmt.Sprintf("%s, Temp: %.1f F", weatherResponse.Weather[0].Description, weatherResponse.Main.Temp)
	return weatherText, nil
}

func FetchWeatherData() string {
	weatherData, err := getWeatherFromAPI()
	if err != nil {
		log.Println("Error fetching weather data:", err)
		return " "
	}
	return weatherData
}

func sendWeatherData(port serial.Port) {
	weather := FetchWeatherData()  
	message := weather + "\n"

	_, err := port.Write([]byte(message))  
	if err != nil {
		log.Println("Failed to send weather data:", err)
	}
}

func handleText(w http.ResponseWriter, r *http.Request) {
	if r.Method == "POST" {
		text := r.FormValue("text") + "\n"

		sendWeatherData(port)

		message := text
		log.Println(text)
		_, err := port.Write([]byte(message))
		if err != nil {
			http.Error(w, "Failed to send data to Arduino", http.StatusInternalServerError)
			return
		}

		w.Write([]byte("OK"))
	}
}

func main() {
	port = initSerial()

	http.Handle("/static/", http.StripPrefix("/static/", http.FileServer(http.Dir("static"))))
	http.HandleFunc("/send", handleText)
	http.Handle("/", http.FileServer(http.Dir("static")))

<<<<<<< HEAD
	fmt.Println("Server started at http://localhost:8080")
	log.Fatal(http.ListenAndServe(":8080", nil))
=======
	fmt.Println("Server started on http://localhost:8080")
	log.Fatal(http.ListenAndServe("0.0.0.0:8080", nil))
>>>>>>> d696e55 (changes)
}
