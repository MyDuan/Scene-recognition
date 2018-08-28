# Scene-recognition
using the generative model(LDA) to recognize the scene 

- 1.using the OpencvImageToPatch to cut the image into difference level.

- 2.get the LBP and HOG feature of every level image.

- 3.link the feature of same image（but different level）, and use kmeans to get the directionary.
- 4.use LDA to get the means feature.
- 5.Train SVM.
- 6.using the test sample to test the learning machine.

This code is worte to use LBP+HOG+SPM to get the local feature then use kmeans to get the words directnionary.Then use LDA to get the means 
feature of every image.Finally use SVM to recognaze the different scene.
