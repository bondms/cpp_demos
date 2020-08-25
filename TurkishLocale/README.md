# Turkish locale

Turkish has four 'i' characters, upper and lower case, with and without a dot.

Care is required for case conversion. Normally, converting "C:\Windows" to upper case would give "C:\WINDOWS", but with Turkish would give "C:\WİNDOWS". On Microsoft Windows, this Turkish conversion results in a different path.

## Install the Turkish language pack
`sudo apt install language-pack-tr`

## Determine which locales are available
`locale -a`

## Environment
From https://en.cppreference.com/w/cpp/locale/setlocale

> During program startup, the equivalent of std::setlocale(LC_ALL, "C"); is executed before any user code is run.

So, the locale of the environment should not affect program behaviour.

## Reference(s)
https://www.thomas-krenn.com/en/wiki/Configure_Locales_in_Ubuntu
