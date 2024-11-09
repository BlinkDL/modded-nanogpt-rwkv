This is a replication attempt for the record attempt described [here](https://x.com/leloykun/status/1854557419768254915).

The original record could not be directly accepted because it showed a slower wallclock time than the previous record -
however, this was plausibly due to hardware differences, where the competitor's hardware as slightly slower.

The [original record attempt's reproducible log](https://github.com/leloykun/modded-nanogpt/blob/224f10d190677d9dc3c9c45da280078196a6fe40/records/110724_EmbeddingBetasCooldown/6c9d875b-ad91-46c9-9ede-2c7f998b9b16.txt)) attained a val loss of 3.2798, just barely below the 3.28 threshold.

To attempt to certify this record, I performed a reproduction (with no modifications) on my own hardware.
This indeeded yielded a new wallclock record (by about 11 seconds), but unfortunately my run attained a worse val loss of 3.2824.

This indicates that the true average val loss of the run may be worse than 3.28, meaning I am **unable to certify it as the new record.**

Ideally, all records should attain a val loss low enough where >95% of runs attain below 3.28. Good evidence for this would be a single run
attaining <= 3.278. Previous records have adhered to this rule, but admittedly it's hard to define precisely and is therefore mostly a matter of taste.
