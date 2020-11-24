import pdfjs
import sys

def main():
   pdf_file = "/path/to/pdf_file"
   pje = pdfjs.PdfJsExtractor();
   # Perform the scanning
   if not pje.searchDocument(pdf_file):
      # Scanning unsuccesful, handle the error
      sys.exit(1);
   else:
      # Scanning successful, iterate through JavaScript scripts in this PDF file
      for i in range(0, pje.getJsCount()):
         # Do something with the script, for example write to stdout
         print pje.getExtractedJs(i)
      
   # Done using PdfJsExtractor

if __name__ == '__main__':
    main()
