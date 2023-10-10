# Honeywell Home API with openHAB using just rules

https://community.openhab.org/t/honeywell-home-api-with-openhab-using-just-rules/123318

As we are all aware, there is not a binding available for the Honeywell Home API. Every now and then over the past few weeks I’ve spent a little bit of time trying to access and interact with their REST API using OH rules and I finally made a break through.

This tutorial is to show what I’ve done in the hopes that it’s useful to someone. I am not suggesting that this approach is the best nor that anyone should pursue it. But it is a good example of what is possible. Ideally this would be written into a binding. Perhaps some day I will get around to doing just that, but don’t let that stop anyone who has time to start on such a binding now.

## Prerequisites

A Honeywell Home device like a thermostat.
A myopenhab.org 3 account and OH successfully connected to it (for the OAuth2 interactions).
Create the app
Navigate to https://developer.honeywellhome.com/ 9 and create an account by clicking on “Sign Up”.

Once logged in click on “Create new APP” with what ever name you want to give it.

Make the “Callback URL” be https://myopenhab.org/static/oauth2.html 7. If you have exposed your OH to the internet, use that URL instead. We will populate the oauth2.html file in the next step.

Once created the app should be approved and you will have a Consumer Key and Consumer Secret. We will need these later.

That’s all we need to do on the Honeywell side.

## OAuth2 Flow

The authentication flow is as follows:

User navigates to a URL, logs into their Honeywell Home account, and gives access to the devices.
Once done Honeywell will make a call to the Callback URL with an auth code.
The auth code is used to request an auth_token and refresh_token.
The auth_token is used in subsequent requests. The auth_token only lasts for a given amount of time. When it expires a new pair of tokens can be requested with the refresh_token.

## Callback

We need a way to receive that auth code that Honeywell will send to us via the callback. To receive and process that place the following HTML into $OH_CONF/html/oauth2.html. This contains a little bit of JavaScript to parse out the auth token from the callback and command the Honeywell_AuthCode Item in OH.

## Items

We will need a bunch of Items to store a lot of this information. Configure these Items with restoreOnStartup .

The AuthCode Item gets populated by the OAuth2 Catcher code above. APIKey and APISecret need to be populated manually. See OH 3 Examples: How to boot strap the state of an Item 2 for a way to do that.

The AccessToken, RefreshToken and Expiration will be populated when a request is made with the AuthCode or RefreshToken to get a new one.

These are the Items needed to handle the authentication and authorization necessary to get the AuthToken.

## Rules

These rules can be written in any language, even Rules DSL. Because the API is mostly returning JSON I’m using JavaScript.

Until recently the HTTP binding was unable to request the state for an Item using a POST or PUT so I’ve implemented the HTTP calls using the openHAB HTTP Actions.

## Library

There are a couple of things that will need to be done over and over from multiple rules: encode the authorzation and process the results from a request for a new AuthToken. I’ve implemented these in a library that gets loaded in the rules that need them.
