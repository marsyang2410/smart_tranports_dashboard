# Smart Transports Dashboard

A real-time bus and MRT ETA dashboard built using the TDX (Taiwan's Transport Data eXchange) API. This project aims to provide users with instant updates on bus and MRT arrival times, with an easy-to-use interface.

## Features

- **Real-Time ETA Display**: Fetches ETA data for buses and MRTs from the TDX API.
- **Auto-Refreshing**: Automatically refreshes every 60 seconds to display up-to-date arrival times.
- **Station and Route Mapping**: Uses StationUID to fetch and map all relevant bus and MRT stops.
- **Error Handling and Debugging**: Full error logs and detailed output to help debug requests and responses.

## Setup

1. **Install Dependencies**:
   - C++ compiler (g++, clang, etc.)
   - libcurl: for making HTTP requests
   - cJSON: for parsing JSON responses

2. **Set Up API Credentials**:
   - Obtain API credentials by signing up for a TDX account [here](https://www.transportdata.tw).
   - Add your `TDX_CLIENT_ID` and `TDX_CLIENT_SECRET` as environment variables:
     ```bash
     export TDX_CLIENT_ID="your_client_id"
     export TDX_CLIENT_SECRET="your_client_secret"
     ```

3. **Build & Run**:
   - Clone the repository and navigate to the project directory.
   - Run `make` to compile the code.
   - Run the program using:
     ```bash
     ./BusTimer
     ```

## Tools Used

- **C++**: Main programming language for backend development.
- **libcurl**: For making HTTP requests to the TDX API.
- **cJSON**: For parsing and processing JSON data from API responses.
- **OAuth2**: For secure token-based authentication with the TDX API.

## Future Features

- **GUI Implementation**: Transitioning from console-based to a graphical interface using Qt or C# WPF.
- **Multiple Station Tracking**: Ability to track multiple bus stations simultaneously.
- **Push Notifications**: Push notifications for ETA updates when the bus is close.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
