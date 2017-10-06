[%bs.raw {|require('./app.css')|}];

open Bs_fetch;

open Data;

let strEl = ReasonReact.stringToElement;

let arrEl = ReasonReact.arrayToElement;

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

let getTargetValue event => (ReactDOMRe.domElementToObj (ReactEventRe.Form.target event))##value;

type state = {forecast: option Data.weatherForecast, place: option string};

type action =
  | UpdatePlace string
  | ForecastResponse Data.weatherForecast;

let component = ReasonReact.reducerComponent "App";

let make _children => {
  ...component,
  initialState: fun () => ({forecast: None, place: None}: state),
  reducer: fun action state =>
    switch action {
    | ForecastResponse forecast => ReasonReact.Update {...state, forecast: Some forecast}
    | UpdatePlace place => ReasonReact.Update {...state, place: Some place}
    },
  didMount: fun _self => {
    Js.log "Component mounted";
    ReasonReact.NoUpdate
  },
  render: fun self => {
    let entries =
      switch self.state.forecast {
      | Some x => x.entries
      | None => [||]
      };
    let listItems =
      Array.map
        (
          fun item =>
            <li> (strEl (item.dt_txt ^ " - " ^ string_of_float item.main.temp ^ " degrees")) </li>
        )
        entries;
    <div className="App">
      <div className="App-header"> <h2> (strEl "Reasonable Weather") </h2> </div>
      <div className="App-content">
        <h3>
          (
            strEl (
              switch self.state.forecast {
              | Some forecast =>
                "Showing results for: " ^ forecast.city.name ^ ", " ^ forecast.city.country
              | None => "Search for a location"
              }
            )
          )
        </h3>
        <p>
          <input
            className="App-input"
            onInput=(
              self.reduce (
                fun e => {
                  let place = getTargetValue e;
                  if (String.length place > 3) {
                    fetchWeather place (self.reduce (fun forecast => ForecastResponse forecast));
                    UpdatePlace place
                  } else {
                    UpdatePlace place
                  }
                }
              )
            )
          />
        </p>
        (
          if (Array.length entries > 0) {
            <ul className="App-list"> (arrEl listItems) </ul>
          } else {
            ReasonReact.nullElement
          }
        )
      </div>
    </div>
  }
};
