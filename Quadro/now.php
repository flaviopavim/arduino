<?php

//echo date('Y-m-d H:i:s');

// Function to fetch data from an API
function fetchData($url) {
    $response = file_get_contents($url);
    return json_decode($response, true);
}

// API URLs
$bitcoinApi = "https://api.coindesk.com/v1/bpi/currentprice/BRL.json"; // For Bitcoin price
$dollarApi = "https://economia.awesomeapi.com.br/last/USD-BRL"; // For USD to BRL exchange rate

// Fetch data from APIs
$bitcoinData = fetchData($bitcoinApi);
$dollarData = fetchData($dollarApi);

// Parse Bitcoin price in BRL
$bitcoinPriceBRL = $bitcoinData['bpi']['BRL']['rate_float'] ?? 0;

// Parse USD to BRL exchange rate
$usdToBRL = $dollarData['USDBRL']['bid'] ?? 0;

// Calculate Bitcoin price in USD
$bitcoinPriceUSD = $usdToBRL > 0 ? $bitcoinPriceBRL / $usdToBRL : 0;

// Get current timestamp
$currentDateTime = date("Y-m-d H:i:s");

// Store previous Bitcoin price (you can persist this in a database or session)
session_start();
$previousBitcoinPrice = $_SESSION['previous_bitcoin_price'] ?? $bitcoinPriceBRL;
$_SESSION['previous_bitcoin_price'] = $bitcoinPriceBRL;

// Calculate percentage variation
$priceVariation = $previousBitcoinPrice > 0 
    ? (($bitcoinPriceBRL - $previousBitcoinPrice) / $previousBitcoinPrice) * 100 
    : 0;

// Create the final array
$data = [
    'datetime' => $currentDateTime,
    'bitcoin' => number_format($bitcoinPriceBRL, 2, '.', ''),
    'bitcoin_usd' => number_format($bitcoinPriceUSD, 2, '.', ''),
    'usd' => number_format($usdToBRL, 2, '.', ''),
//    'price_variation_percent' => number_format($priceVariation, 2, '.', '')
];

// Output the array as JSON
header('Content-Type: application/json');
echo json_encode($data, JSON_PRETTY_PRINT);