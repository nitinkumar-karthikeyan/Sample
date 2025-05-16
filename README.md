student_id = 'F432447'

The project uses a Grove NTC Thermistor temperature sensor connected to an Arduino Uno. I configured the Arduino to read analog values from the sensor, converted them to Celsius and collected several samples. I wrote a function to perform a Discrete Fourier Transformation (DFT) on the temperature data to find the dominant frequency. This helped me find out how rapidly the temperature is changing.

Task 1.1: Authentication Issues in Google Colab
Reason for the issue:
- Google Colab is not authenticated with Github. The authentication token that is being used does not have the correct permissions.
How Colab access repository?:
- Using OAuth Authentication
Steps to allow Colab to save notebooks:
- Authorise Github in Colab
- Use Github Personal Access Token
How to verify?:
- Save notebook again
- Check Github Account activity

Task 1.3: Unable to Delete a Branch
- Git does not allow you to delete the branch you are currently working on.
- This restriction prevents potential file corruption and loss of reference.
- If the active branch is deleted, Git would have no branch to reference because of deletion and no changes can be committed.
- Once you are on another branch, the unwanted branch can be deleted without any issues

Task 1.4:  Handling a Merge Conflict in Git
- Both branches modified the same file but made different changes to it
- Git prevents automatic merging because it cannot combine conflicting changes. If it were to conflict changes, it might lose important data from the code.
- In order to prevent such merge conflicts:
    - Pull frequently from main branch
    - Review chanegs before merging
- 'git fetch' should be run to update information
- Forcing a push can overwrite remote changes and leave the repository in a broken state.

Task 4:
Memory Utilization in my implementation:
- temperature_value[samples] : Stores 60 temperature samples per cycle
- temp_variations[10]: Stores variation values for last 10 cycles for moving average

Impact of storing large arrays in Arduino:
- Memory fragmentation: Repeated allocation can cause issues even when total free RAM is sufficient
- Limited RAM: Storing large arrays can quickly consume the limited RAM of Arduino, since they have only 2KB of SRAM. I cam across this issue in 3.1-3.4 where I had to code the real and imaginary code into each fucntion that required it.

Reduce Usage:
- Using smaller data types where possible
- Reduce array size

Justification:
Since Arduino's RAM is limited, storing large floating-point arrays for temperature data risks too much RAM. This causes stack overflow and unexpected crashes. My solution to this task does not cross the maximum number of bytes before memory fragmentation. 

Enhancement:
- Instead of storing all 60 variables, it is much easier to process each one as it is processed such as for DFT, I could have used a DFT sliding approach with fixed buffer size.
- This approach reduces memory usage and makes the code more robust.

Files in Repository:
- 24WSA024 JNB Arduino Coursework Final 2025S2(V1).ipynb
- F432447_img (Arduino Hardware Connection image)
- threeminstemperature_F432447.csv (CSV file)
- project-notes.md
