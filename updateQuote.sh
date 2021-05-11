#!/bin/bash
cd /home/pi
curl -s 'https://blockchain.info/ticker' | jq ".EUR.last" > lastQuote.txt && \
	date > lastUpdate.txt

