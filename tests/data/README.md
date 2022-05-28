botan-cli keygen --algo=ECDSA  > test_ecdsa_key.key

botan-cli gen_self_signed test_ecdsa_key.key example.org  --path-limit=1 --days=365   --emsa= > test_ecdsa_certificate.crt
