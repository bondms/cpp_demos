# Turkish locale

Turkish has four 'i' characters, upper and lower case, with and without a dot.

Care is required for case conversion. Normally, converting "C:\Windows" to upper case would give "C:\WINDOWS", but with Turkish would give "C:\WİNDOWS". On Microsoft Windows, this Turkish conversion results in a different path.

## Install the Turkish language pack
`sudo apt install language-pack-tr`

## Determine which locales are available
`locale -a`

## Reference(s)
https://www.thomas-krenn.com/en/wiki/Configure_Locales_in_Ubuntu
