import lxml.etree as et
import os
import sys
import argparse
import logging
import tempfile


import log_helper


__doc__ = """Application transforms XML files and QT translation (*.ts) files using XSL transformation
Input file (XML or TS) should be provided, XSL file should be provided
Output depends on the XSL file contents, it could be HTML-table, it could be tab-delimeted file
opened in Excel, it could be other XML file

Usage examples:

python3 xsl_transform.py --ts translation.ts --xsl xslt/to_excel_table.xsl --output translation.txt

python3 xsl_transform.py --ts translation.ts --xsl xslt/to_html_table.xsl --output translation.html

python3 xsl_transform.py --xml translation.xml --xsl xslt/to_html_table.xsl --output translation.html

You can also perform all transformations online at:
https://www.freeformatter.com/xsl-transformer.html

"""


logger = log_helper.setup_logger(name="xsl_transform", level=logging.DEBUG, log_to_file=False)


TEMP_XML = "6b6fce27-4b3f-497b-a291-8f966d152de9.xml"


def try_remove(filename):
    """
    Try remove filename, if does not exist do nothing
    :param filename:
    """
    try:
        os.remove(filename)
        logger.info("Removed tmp file {0}".format(filename))
    except OSError as e:
        logger.info("Failed to remove tmp file {0} with error {1}".format(filename, e))
        pass


class XslTransformer:
    """
    Describes input file (TS or XML), XSL transformation and output file
    """

    def __init__(self, transform_xsl, output, input_xml='', input_ts=''):
        if input_xml=='':
            self.input_xml = input_ts
        elif input_ts=='':
            self.input_xml = input_xml
        self.transform_xsl = transform_xsl
        self.output = output
        self.temp_dir = tempfile.gettempdir()

    def convert_ts(self):
        logger.info("Tempdir is {0}".format(self.temp_dir))
        temp_xml_path = os.path.join(self.temp_dir, TEMP_XML)
        logger.info("Temp XML is {0}".format(temp_xml_path))
        temp_file = open(temp_xml_path, 'w', encoding="utf-8")
        try_remove(temp_xml_path)
        with open(self.input_xml, "r", encoding="utf-8") as xml_lines:
            # <?xml version="1.0" encoding="utf-8"?>
            for line in xml_lines:
                if line == "<!DOCTYPE TS>\n":
                    logger.info("DOCTYPE tag found")
                else:
                    temp_file.write(line)
        temp_file.close()
        self.input_xml = temp_xml_path

    def transform(self):
        xml_file = et.parse(self.input_xml)
        logger.info("Parsed {0}".format(self.input_xml))
        xslt_file = et.parse(self.transform_xsl)
        logger.info("Parsed {0}".format(self.transform_xsl))
        transform = et.XSLT(xslt_file)
        logger.info("Performed transformation")
        transformed = transform(xml_file)
        with open(self.output, "w") as out_file:
            print((et.tostring(transformed, pretty_print=True)).decode("utf-8"), file=out_file)
        logger.info("Transformed data has written to {0}".format(self.output))


def main():
    """
    Read, read and parse, or read, parse and send release_notes.json file
    :return: Exec return code
    """
    parser = argparse.ArgumentParser(description='Command-line interface')
    parser.add_argument('--ts',
                        help='Set TS file (Qt translation file) for transformation',
                        dest='ts',
                        default='',
                        required=False)

    parser.add_argument('--xml',
                        help='Set XML file for transformation',
                        dest='xml',
                        default='',
                        required=False)

    parser.add_argument('--xsl',
                        help='XSL transformation file, describing applied transformation rules',
                        dest='xsl',
                        required=True)

    parser.add_argument('--output',
                        help='Transformed file, could have any textual format (XML, HTML, CSV, TXT...)',
                        dest='output',
                        default='output.txt',
                        required=False)

    args = parser.parse_args()

    if args.ts == "" and args.xml == "":
        logger.warning("You should pass some input file")

    if args.ts == "":
        xsl_transformer = XslTransformer(input_xml=args.xml, transform_xsl=args.xsl, output=args.output)
        xsl_transformer.transform()
    elif args.xml == "":
        xsl_transformer = XslTransformer(input_ts=args.ts, transform_xsl=args.xsl, output=args.output)
        xsl_transformer.convert_ts()
        xsl_transformer.transform()
    return 0


###########################################################################
if __name__ == '__main__':
    sys.exit(main())
