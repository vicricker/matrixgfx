<div id="top"></div>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <!-- <a href="https://github.com/vicricker/matrixgfx">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a> -->

  <h3 align="center">matrixgfx</h3>

</div>



<!-- ABOUT THE PROJECT -->
## About The Project

<!-- [![Product Name Screen Shot][product-screenshot]](https://example.com) -->

This project contains a library with some drawing functions, and some sample apps:

### Fire

Draws a simple fire effect

![Fire](/images/fire.png)

### Plasma

Draws a simple plasma effect

![Plasma](/images/plasma.png)

### Clock

Draws a clock with a 7 segment LED style font.  Some sprites walk across the screen occasionally.  At 10s before midnight New Year's eve, it will countdown the last 10 seconds of the year, then disaplay fireworks and scroll "HAPPY NEW YEAR" for 10 minutes.  Technically, it runs every night at 11:59:50 because I am lazy and need to fix it.

![Clock](/images/clock.png)

### Scroller

Demo of scrolling text.

![Scroller Demo](/images/scroller.png)

### matrixgfx library

Has some functions for blitting transparent images, drawing scrolling text, generating color gradients, etc.  Maybe I should write some documentation?

<p align="right">(<a href="#top">back to top</a>)</p>



### Built With

* [rpi-rgb-led-matrix](https://github.com/hzeller/rpi-rgb-led-matrix)
* [CMake](https://cmake.org/)

<p align="right">(<a href="#top">back to top</a>)</p>


### Prerequisites

* Raspberry Pi
* RGB Matrix and Driver hat
* cmake, and the c compiler can be installed with the package manager
* rpi-rgb-led-matrix libray

I'm using a Pi 3 with two 64x32 panels chained together as 128x32

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/vicricker/matrixgfx.git
   ```
2. Build the code
   ```sh
   cd matrixgfx
   mkdir build
   cd build
   cmake ..
   make
   ```
3. Some of the apps reference the fonts and sprites from the current directory. You can link them into the build directory to make things easier.
    ```sh
    cd build
    ls -s ../fonts
    ln -s ../sprites
    ```

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
## Usage

```sh
sudo ./fire --led-cols=128 --led-rows=32 --led-gpio-mapping=adafruit-hat-pwm --led-show-refresh --led-limit-refresh=120
```

All of the apps accept the command line options supported by rpi-rgb-led-matrix.

I use --led-gpio-mapping=adafruit-hat-pwm which requires a hardware mod to the hat.  Also, I might have hardcoded things in the rpi-rgb-led-matrix library for my hardware.  I forget. You may need to alter the configration for your environment.

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Vic Ricker - vic@ricker.us

Project Link: [https://github.com/vicricker/matrixgfx](https://github.com/vicricker/matrixgfx)

<p align="right">(<a href="#top">back to top</a>)</p>




<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/vicricker/matrixgfx.svg?style=for-the-badge
[contributors-url]: https://github.com/vicricker/matrixgfx/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/vicricker/matrixgfx.svg?style=for-the-badge
[forks-url]: https://github.com/vicricker/matrixgfx/network/members
[stars-shield]: https://img.shields.io/github/stars/vicricker/matrixgfx.svg?style=for-the-badge
[stars-url]: https://github.com/vicricker/matrixgfx/stargazers
[issues-shield]: https://img.shields.io/github/issues/vicricker/matrixgfx.svg?style=for-the-badge
[issues-url]: https://github.com/vicricker/matrixgfx/issues
[license-shield]: https://img.shields.io/github/license/vicricker/matrixgfx.svg?style=for-the-badge
[license-url]: https://github.com/vicricker/matrixgfx/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/victor-ricker-875472b
[product-screenshot]: images/screenshot.png