import requests
import numpy as np
if __name__ == "__main__":
    request_data = {
	"inputs": [{
		"name": "DALI_INPUT_0",
		"shape": [1, 5],
		"datatype": "UINT32",
		"data": [
        [1, 1, 1, 1, 1]
        ]
	}],
	"outputs": [{"name": "DALI_OUTPUT_0"}]
}
    res = requests.post(url="http://localhost:8000/v2/models/mydali/versions/1/infer",json=request_data).json()
    print(res)