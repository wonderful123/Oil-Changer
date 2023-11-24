import json
from jsonschema import validate, ValidationError

def load_json(filename):
    with open(filename, 'r') as file:
        return json.load(file)

def validate_json(data, schema):
    try:
        validate(instance=data, schema=schema)
        print("Validation successful.")
    except ValidationError as e:
        print("Validation error:", e)

if __name__ == "__main__":
    config = load_json('../data/config/hardwareConfig.json')
    schema = load_json('hardwareConfig-schema.json')  # Assuming your schema is stored in schema.json
    validate_json(config, schema)
