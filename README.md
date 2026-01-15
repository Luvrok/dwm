## dwm - Luvrok's build
inspired by [bread's build](https://github.com/BreadOnPenguins/dwm), [bakkeby's dwm-flexipatch](https://github.com/bakkeby/dwm-flexipatch), [vanilla dwm](https://git.suckless.org/dwm/files.html) and [my own old dwm-flexipatch build](https://github.com/Luvrok/dwm-flexipatch).

## patches applied:
* [awesomebar (dwm-awesomebar-20250923-6.6.diff )](https://dwm.suckless.org/patches/awesomebar/)
* [noborder (dwm-noborderselflickerfix-2022042627-d93ff48803f0.diff)](https://dwm.suckless.org/patches/noborder/)
* [pertag (dwm-pertag-20200914-61bb8b2.diff)](https://dwm.suckless.org/patches/pertag/)
* [statuscmd (dwm-statuscmd-20241009-8933ebc.diff)](https://dwm.suckless.org/patches/statuscmd/)
* [togglelayout (dwm-togglelayout-6.6.diff)](https://github.com/bakkeby/patches/blob/master/dwm/dwm-togglelayout-6.6.diff)
* [my fix togglelayout compatibility with pertag (dwm-togglelayout-6.6-pertag-fix.diff)](https://github.com/Luvrok/dwm/blob/master/patch/dwm-togglelayout-6.6-pertag-fix.diff)
* [steam (dwm-steam-6.2.diff)](https://dwm.suckless.org/patches/steam/dwm-steam-6.2.diff)
* [actualfullscreen (dwm-actualfullscreen-20211013-cb3f58a.diff)](https://dwm.suckless.org/patches/actualfullscreen/)
* [hide vacant tags (dwm-hide_vacant_tags-6.4.diff)](https://dwm.suckless.org/patches/hide_vacant_tags/)
* [vanitygaps (dwm-vanitygaps-20200610-f09418b.diff)](https://dwm.suckless.org/patches/vanitygaps/)
* [fixmultimon (dwm-fixmultimon-6.4.diff)](https://dwm.suckless.org/patches/fixmultimon/)
* [focusmaster (dwm-focusmaster-return-6.2.diff)](https://dwm.suckless.org/patches/focusmaster/)
* [focusmonmouse (dwm-focusmonmouse-6.2.diff)](https://dwm.suckless.org/patches/focusmonmouse/)
* [stacker (dwm-stacker-6.6.diff)](https://dwm.suckless.org/patches/stacker/)
* [sticky (dwm-sticky-6.5.diff)](https://dwm.suckless.org/patches/sticky/)
* [floatrules (dwm-floatrules-20210801-138b405.diff)](https://dwm.suckless.org/patches/floatrules/)
* [xrdb (dwm-xrdb-6.4.diff)](https://dwm.suckless.org/patches/xrdb/)
* [destroyfocus (dwm-destroyfocus-20210329-61bb8b2.diff)](https://dwm.suckless.org/patches/destroyfocus/)
* [selfrestart (dwm-r1615-selfrestart.diff)](https://dwm.suckless.org/patches/selfrestart/)
modified for NixOS, see [self-restart (NixOS)](#self-restart-nixos)
* [center (dwm-center-6.2.diff)](https://dwm.suckless.org/patches/center/)

## also check:
### prioritize focus under the mouse
ensures focus follows the mouse when switching tags and after client destruction.

related commits: [1](https://github.com/Luvrok/dwm/commit/4d4688bc3990a05ee86a0e1c45bf136034774e67), [2](https://github.com/Luvrok/dwm/commit/1f664afdd5dde743bb5d2542ff7b33e4eec51ead), [3](https://github.com/Luvrok/dwm/commit/bf7df3f61daa697af5ee54676e1f1ee4cb74b516)
### self-restart (NixOS)
updated self-restart implementation for NixOS using a more correct dwm reload approach.
also adds a keybinding and a helper function to launch a test dwm binary from a predefined path.

related commits:
[1](https://github.com/Luvrok/dwm/commit/9df57752f7aea890964f24182e5ddcdab0ce2221), [2](https://github.com/Luvrok/dwm/commit/d132d2b82b5f2bbd4ce5a8ebd27281a7a470f3cd)
