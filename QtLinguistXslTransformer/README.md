## QtLinguistXslTransformer

Application transforms XML files and QT translation (*.ts) files using XSL transformation
Input file (XML or TS) should be provided, XSL file should be provided
Output depends on the XSL file contents, it could be HTML-table, it could be tab-delimeted file
opened in Excel, it could be other XML file

Usage examples:

`python3 xsl_transform.py --ts translation.ts --xsl xslt/to_excel_table.xsl --output translation.txt`

`python3 xsl_transform.py --ts translation.ts --xsl xslt/to_html_table.xsl --output translation.html`

`python3 xsl_transform.py --xml translation.xml --xsl xslt/to_html_table.xsl --output translation.html`
