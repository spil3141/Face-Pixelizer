"""
python3 detect.py \
  --model ${TEST_DATA}/mobilenet_ssd_v2_face_quant_postprocess_edgetpu.tflite
"""

import argparse
import os
import numpy as np
import tensorflow as tf
import numpy as np
import PIL 
import matplotlib.pyplot as plt 
import matplotlib.image as matimage

class ConvolutionalAutoencoder(tf.keras.models.Model):
  def __init__(self):
    super(ConvolutionalAutoencoder,self).__init__()
    self.encoder_input_shape = (128,128,3)
    self.encoder = tf.keras.models.Sequential([
        tf.keras.layers.Input(shape= self.encoder_input_shape),
        tf.keras.layers.Conv2D(16, (3,3), activation='relu', padding='same'),
        tf.keras.layers.MaxPooling2D(2,2),
        tf.keras.layers.Conv2D(8, (3,3), activation='relu', padding='same'),
        tf.keras.layers.MaxPooling2D(2,2),
        tf.keras.layers.Conv2D(3, (3,3), activation='relu', padding='same'),
    ])

    self.decoder = tf.keras.Sequential([
      # Upsample its input 
      tf.keras.layers.UpSampling2D((2, 2)),
#       tf.keras.layers.Conv2D(3, kernel_size=(3,3),strides=2, activation='relu', padding='same'),
      tf.keras.layers.Conv2D(3, kernel_size=(3,3),strides=2, activation='sigmoid', padding='same')])

  def call(self, x):
    encoded = self.encoder(x)
    decoded = self.decoder(encoded)
    return decoded

def remove_alpha(img : np) -> np:
    return np.array([img[0,:,:,:3]])

def display_sample(img : np):
    plt.imshow(img[0])
    plt.show()

def Get_Img(path : str) -> np:
    img_2 = np.asarray(PIL.Image.open(path).resize((128,128)))
    img_2 = np.array([img_2])
    img_2 = img_2/255
    print("shape: ", img_2.shape)
    if img_2.shape[-1] >3:
        img_2 = remove_alpha(img_2)
    return img_2

def Save(imgarray : np, path : str) -> None:
    # method 1
    matimage.imsave(os.path.join(path,"output.png"),imgarray)
    #method 2 (not working)
    # imgarray = imgarray * 255 
    # imgarray = imgarray.astype(int)
    # imgarray = PIL.Image.fromarray(imgarray)
    # imgarray.save(os.path.join(path,"output.png"))
                    
def main():
    default_encoder_model = 'res/python/res/SavedModels/pretrained_model_encoder.h5'
    default_decoder_model = 'res/python/res/SavedModels/pretrained_model_decoder.h5'
    image_output_dir = "res/python/res/data/output"
    parser = argparse.ArgumentParser()
    parser.add_argument('--use_model', type=bool, default=True, help='Use default model?')
    parser.add_argument("--img", help=" The relative path of the targeted image to this file.",
                        default= "None")
    parser.add_argument("--display", help=" Display result", default = False)
    parser.add_argument("--output_dir", help="The output directory.", default = None)
    args = parser.parse_args()
    
    if(args.use_model and args.img != "None"):
        print("Using pretrained model.")
    else:
        print("No Pretrained Model Selected")
        return
    
    pixelazer = ConvolutionalAutoencoder()
    
    # Loading Pretrained Model
    pixelazer.encoder = tf.keras.models.load_model(default_encoder_model)
    pixelazer.decoder = tf.keras.models.load_model(default_decoder_model)
    
    pixelazer.compile(optimizer= "adam", loss=tf.keras.losses.MeanSquaredError())

    output = pixelazer.predict(Get_Img(args.img))
    
    if(args.display):
        display_sample(output)
    
    if(args.output_dir != None):
        Save(output[0],args.output_dir)
    print("Done")
    exit()

if __name__ == '__main__':
    main()