[%bs.raw {|require('./app.css')|}];

open Bs_fetch;

type state = {forecast: option Data.weatherForecast};

let makeForecastUrl place =>
  "https://api.openweathermap.org/data/2.5/forecast?q=" ^
  place ^ "&units=metric&APPID=" ^ Keys.apiKey;

let fetchWeather place callback =>
  Js.Promise.(
    fetch (makeForecastUrl place) |> then_ Response.text |>
    then_ (
      fun jsonText => {
        callback (Js.Json.parseExn jsonText |> Data.parseWeatherForecast);
        resolve ()
      }
    ) |> ignore
  );

type action =
  | ForecastResponse Data.weatherForecast;

let component = ReasonReact.reducerComponent "App";

let make _children => {
  ...component,
  initialState: fun () => ({forecast: None}: state),
  reducer: fun action state =>
    switch action {
    | ForecastResponse forecast => ReasonReact.Update {forecast: Some forecast}
    },
  didMount: fun self => {
    Js.log "Component mounted";
    fetchWeather "London" (self.reduce (fun forecast => ForecastResponse forecast));
    ReasonReact.NoUpdate
  },
  render: fun {state} =>
    <div className="App">
      <div className="App-header">
        <h2> (ReasonReact.stringToElement "Reasonable Weather") </h2>
      </div>
      <p className="App-intro">
        (
          ReasonReact.stringToElement (
            "status: " ^ (
              switch state.forecast {
              | Some _ => "Got it!"
              | None => "Waiting..."
              }
            )
          )
        )
      </p>
    </div>
};
