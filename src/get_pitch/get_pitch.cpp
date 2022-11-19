/// @file

#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>

#include "wavfile_mono.h"
#include "pitch_analyzer.h"

#include "docopt.h"

#define FRAME_LEN   0.030 /* 30 ms. */
#define FRAME_SHIFT 0.015 /* 15 ms. */

using namespace std;
using namespace upc;

static const char USAGE[] = R"(
get_pitch - Pitch Estimator 

Usage:
    get_pitch [options] <input-wav> <output-txt>
    get_pitch (-h | --help)
    get_pitch --version

Options:
    -m REAL, --umaxnorm=REAL  Llindar del maxim de l'autocorrelacio [default: 0.4]
    -h, --help  Show this screen
    --version   Show the version of the project

Arguments:
    input-wav   Wave file with the audio signal
    output-txt  Output file: ASCII file with the result of the estimation:
                    - One line per frame with the estimated f0
                    - If considered unvoiced, f0 must be set to f0 = 0
)";

int main(int argc, const char *argv[]) {
	/// \TODO 
	///  Modify the program syntax and the call to **docopt()** in order to
	///  add options and arguments to the program.
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
        {argv + 1, argv + argc},	// array of arguments, without the program name
        true,    // show help if requested
        "2.0");  // version string

	std::string input_wav = args["<input-wav>"].asString();
	std::string output_txt = args["<output-txt>"].asString();
  float umaxnorm = stof(args["--umaxnorm"].asString());
  
  
  

  // Read input sound file
  unsigned int rate;
  vector<float> x;
  if (readwav_mono(input_wav, rate, x) != 0) {
    cerr << "Error reading input file " << input_wav << " (" << strerror(errno) << ")\n";
    return -2;
  }

  int n_len = rate * FRAME_LEN;
  int n_shift = rate * FRAME_SHIFT;

  // Define analyzer
  PitchAnalyzer analyzer(n_len, rate, PitchAnalyzer::RECT, 50, 500,umaxnorm);

  /// \TODO
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
  
  // Iterate for each frame and save values in f0 vector
  vector<float>::iterator iX;
  float max=0;
  for(iX=x.begin(); iX < x.begin()+(int) x.size(); iX++){
    if(abs(*iX) > max)
      max=*iX;
  }
  float th=0.01*max;
  for(iX=x.begin(); iX < x.begin()+(int) x.size(); iX++){
    if(abs(*iX)<th){
      *iX=0;
    }else{
      if(*iX>0){
        *iX=*iX-th;
      }else{
        *iX=*iX+th;
      }
    }
  }

  vector<float> f0;
  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
    float f = analyzer(iX, iX + n_len);
    f0.push_back(f);
  }

  /// \TODO
  int F_size = 1;  
  vector<float> filter; 
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.
  for(iX = f0.begin(); iX<f0.end()-(F_size-1); iX += 1){
    // fill filter    
    for(int i = 0; i<F_size; i++)      
      filter.push_back(*(iX+i));
    // Order filter  ​
    int k, l;

    for(k = 0; k < F_size-1; k++){
      for(l = 0; l < F_size-k-1; l++){
        if (filter[l] > filter[l+1]){        
          float aux = filter[l];        
          filter[l] = filter[l+1]; 
          filter[l+1] = aux;      
        }    
      }   
    }
    // Get median    
    f0[iX - f0.begin()] = filter[F_size/2];
    filter.clear();  
  } 
  // Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iX = f0.begin(); iX != f0.end(); ++iX) 
    os << *iX << '\n';
  os << 0 << '\n';//pitch at t=Dur

  return 0;
}
