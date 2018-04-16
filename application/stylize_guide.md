# How to produce a stylized video

## Clone the artistic-videos Repository

```bash
git clone https://github.com/manuelruder/artistic-videos
```

## Put Deepmatch and Deepflow in

Compile the parallelized deepflow, put the executable into the cloned folder. Download [Deepmatching](http://lear.inrialpes.fr/src/deepmatching/) (You might want to try their GPU version but we don't have luck to make their GPU version work (a lot of compile errors))

Then edit `run-deepflow.sh` with the correct executable path.

## Run

The rest of the steps should be same with the descriptions in the original README of `artistic-videos`. Basically, you just replace the old deepflow implementation with our parallelized implementation.
