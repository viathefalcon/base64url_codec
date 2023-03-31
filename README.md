# Base64URL (En)coder/decoder

My current work involves dealing with quite a bit of [Base64URL-encoded data](https://jwt.io/), so I thought it would be useful to have some tools that can work with it, locally and natively (on Windows, macOS and Linux).

The encoder and decoder both read from _stdin_ and write to _stdout_.

## Example Usage
### Decoding the payload of a JWT
(and pretty-printing it with [jq](https://stedolan.github.io/jq/))
```sh
awk -F '.' '{print $2}' example.jwt | base64urldecode | jq
```
