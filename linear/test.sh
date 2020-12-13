#!/bin/bash


./test_main ./docs/doc.txt > ./docs/doc1_result
./test_main ./docs/doc2.txt > ./docs/doc2_result


grep -vxF -f ./docs/doc1_result ./docs/doc2_result