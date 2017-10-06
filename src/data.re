let jsonStr = {|{
  "cod": "200",
  "message": 0.0247,
  "cnt": 40,
  "list": [
    {
      "dt": 1507237200,
      "main": {
        "temp": 11.49,
        "temp_min": 11.49,
        "temp_max": 11.87,
        "pressure": 1027.51,
        "sea_level": 1035.2,
        "grnd_level": 1027.51,
        "humidity": 66,
        "temp_kf": -0.38
      },
      "dt_txt": "2017-10-05 21:00:00"
    }
  ],
  "city": {
    "id": 2643743,
    "name": "London",
    "coord": {
      "lat": 51.5085,
      "lon": -0.1258
    },
    "country": "GB"
  }
}|};

type weatherItemMain = {temp: float, pressure: float, humidity: int};

type weatherItem = {dt: int, main: weatherItemMain, dt_txt: string};

type city = {name: string, country: string};

type weatherForecast = {list: array weatherItem, city};

let parseWeatherItemMain json :weatherItemMain =>
  Json.{
    temp: json |> Decode.field "temp" Decode.float,
    pressure: json |> Decode.field "pressure" Decode.float,
    humidity: json |> Decode.field "humidity" Decode.int
  };

let parseWeatherItem json :weatherItem =>
  Json.{
    dt: json |> Decode.field "dt" Decode.int,
    dt_txt: json |> Decode.field "dt_txt" Decode.string,
    main: json |> Decode.field "main" parseWeatherItemMain
  };

let parseCity json :city =>
  Json.{
    name: json |> Decode.field "name" Decode.string,
    country: json |> Decode.field "country" Decode.string
  };

let parseWeatherForecast json :weatherForecast =>
  Json.{
    list: json |> Decode.field "list" (Decode.array parseWeatherItem),
    city: json |> Decode.field "city" parseCity
  };

let exampleForecast = parseWeatherForecast (Js.Json.parseExn jsonStr);
