#pragma once

class WeightedHandTable
{
public:
	WeightedHandTable(void);
	~WeightedHandTable(void);

	void SetWeight(unsigned char card1, unsigned char card2, double weight);
	double GetWeight(unsigned char card1, unsigned char card2);

private:
	int GetIndex(unsigned char card1, unsigned char card2);

	double	hand_table[1326];	// Array of all unique hole card combinations
};
/*
for convenience:
0	2h2d
1	2h2c
2	2h2s
3	2d2c
4	2d2s
5	2c2s
6	3h2h
7	3h2d
8	3h2c
9	3h2s
10	3d2h
11	3d2d
12	3d2c
13	3d2s
14	3c2h
15	3c2d
16	3c2c
17	3c2s
18	3s2h
19	3s2d
20	3s2c
21	3s2s
22	3h3d
23	3h3c
24	3h3s
25	3d3c
26	3d3s
27	3c3s
28	4h2h
29	4h2d
30	4h2c
31	4h2s
32	4d2h
33	4d2d
34	4d2c
35	4d2s
36	4c2h
37	4c2d
38	4c2c
39	4c2s
40	4s2h
41	4s2d
42	4s2c
43	4s2s
44	4h3h
45	4h3d
46	4h3c
47	4h3s
48	4d3h
49	4d3d
50	4d3c
51	4d3s
52	4c3h
53	4c3d
54	4c3c
55	4c3s
56	4s3h
57	4s3d
58	4s3c
59	4s3s
60	4h4d
61	4h4c
62	4h4s
63	4d4c
64	4d4s
65	4c4s
66	5h2h
67	5h2d
68	5h2c
69	5h2s
70	5d2h
71	5d2d
72	5d2c
73	5d2s
74	5c2h
75	5c2d
76	5c2c
77	5c2s
78	5s2h
79	5s2d
80	5s2c
81	5s2s
82	5h3h
83	5h3d
84	5h3c
85	5h3s
86	5d3h
87	5d3d
88	5d3c
89	5d3s
90	5c3h
91	5c3d
92	5c3c
93	5c3s
94	5s3h
95	5s3d
96	5s3c
97	5s3s
98	5h4h
99	5h4d
100	5h4c
101	5h4s
102	5d4h
103	5d4d
104	5d4c
105	5d4s
106	5c4h
107	5c4d
108	5c4c
109	5c4s
110	5s4h
111	5s4d
112	5s4c
113	5s4s
114	5h5d
115	5h5c
116	5h5s
117	5d5c
118	5d5s
119	5c5s
120	6h2h
121	6h2d
122	6h2c
123	6h2s
124	6d2h
125	6d2d
126	6d2c
127	6d2s
128	6c2h
129	6c2d
130	6c2c
131	6c2s
132	6s2h
133	6s2d
134	6s2c
135	6s2s
136	6h3h
137	6h3d
138	6h3c
139	6h3s
140	6d3h
141	6d3d
142	6d3c
143	6d3s
144	6c3h
145	6c3d
146	6c3c
147	6c3s
148	6s3h
149	6s3d
150	6s3c
151	6s3s
152	6h4h
153	6h4d
154	6h4c
155	6h4s
156	6d4h
157	6d4d
158	6d4c
159	6d4s
160	6c4h
161	6c4d
162	6c4c
163	6c4s
164	6s4h
165	6s4d
166	6s4c
167	6s4s
168	6h5h
169	6h5d
170	6h5c
171	6h5s
172	6d5h
173	6d5d
174	6d5c
175	6d5s
176	6c5h
177	6c5d
178	6c5c
179	6c5s
180	6s5h
181	6s5d
182	6s5c
183	6s5s
184	6h6d
185	6h6c
186	6h6s
187	6d6c
188	6d6s
189	6c6s
190	7h2h
191	7h2d
192	7h2c
193	7h2s
194	7d2h
195	7d2d
196	7d2c
197	7d2s
198	7c2h
199	7c2d
200	7c2c
201	7c2s
202	7s2h
203	7s2d
204	7s2c
205	7s2s
206	7h3h
207	7h3d
208	7h3c
209	7h3s
210	7d3h
211	7d3d
212	7d3c
213	7d3s
214	7c3h
215	7c3d
216	7c3c
217	7c3s
218	7s3h
219	7s3d
220	7s3c
221	7s3s
222	7h4h
223	7h4d
224	7h4c
225	7h4s
226	7d4h
227	7d4d
228	7d4c
229	7d4s
230	7c4h
231	7c4d
232	7c4c
233	7c4s
234	7s4h
235	7s4d
236	7s4c
237	7s4s
238	7h5h
239	7h5d
240	7h5c
241	7h5s
242	7d5h
243	7d5d
244	7d5c
245	7d5s
246	7c5h
247	7c5d
248	7c5c
249	7c5s
250	7s5h
251	7s5d
252	7s5c
253	7s5s
254	7h6h
255	7h6d
256	7h6c
257	7h6s
258	7d6h
259	7d6d
260	7d6c
261	7d6s
262	7c6h
263	7c6d
264	7c6c
265	7c6s
266	7s6h
267	7s6d
268	7s6c
269	7s6s
270	7h7d
271	7h7c
272	7h7s
273	7d7c
274	7d7s
275	7c7s
276	8h2h
277	8h2d
278	8h2c
279	8h2s
280	8d2h
281	8d2d
282	8d2c
283	8d2s
284	8c2h
285	8c2d
286	8c2c
287	8c2s
288	8s2h
289	8s2d
290	8s2c
291	8s2s
292	8h3h
293	8h3d
294	8h3c
295	8h3s
296	8d3h
297	8d3d
298	8d3c
299	8d3s
300	8c3h
301	8c3d
302	8c3c
303	8c3s
304	8s3h
305	8s3d
306	8s3c
307	8s3s
308	8h4h
309	8h4d
310	8h4c
311	8h4s
312	8d4h
313	8d4d
314	8d4c
315	8d4s
316	8c4h
317	8c4d
318	8c4c
319	8c4s
320	8s4h
321	8s4d
322	8s4c
323	8s4s
324	8h5h
325	8h5d
326	8h5c
327	8h5s
328	8d5h
329	8d5d
330	8d5c
331	8d5s
332	8c5h
333	8c5d
334	8c5c
335	8c5s
336	8s5h
337	8s5d
338	8s5c
339	8s5s
340	8h6h
341	8h6d
342	8h6c
343	8h6s
344	8d6h
345	8d6d
346	8d6c
347	8d6s
348	8c6h
349	8c6d
350	8c6c
351	8c6s
352	8s6h
353	8s6d
354	8s6c
355	8s6s
356	8h7h
357	8h7d
358	8h7c
359	8h7s
360	8d7h
361	8d7d
362	8d7c
363	8d7s
364	8c7h
365	8c7d
366	8c7c
367	8c7s
368	8s7h
369	8s7d
370	8s7c
371	8s7s
372	8h8d
373	8h8c
374	8h8s
375	8d8c
376	8d8s
377	8c8s
378	9h2h
379	9h2d
380	9h2c
381	9h2s
382	9d2h
383	9d2d
384	9d2c
385	9d2s
386	9c2h
387	9c2d
388	9c2c
389	9c2s
390	9s2h
391	9s2d
392	9s2c
393	9s2s
394	9h3h
395	9h3d
396	9h3c
397	9h3s
398	9d3h
399	9d3d
400	9d3c
401	9d3s
402	9c3h
403	9c3d
404	9c3c
405	9c3s
406	9s3h
407	9s3d
408	9s3c
409	9s3s
410	9h4h
411	9h4d
412	9h4c
413	9h4s
414	9d4h
415	9d4d
416	9d4c
417	9d4s
418	9c4h
419	9c4d
420	9c4c
421	9c4s
422	9s4h
423	9s4d
424	9s4c
425	9s4s
426	9h5h
427	9h5d
428	9h5c
429	9h5s
430	9d5h
431	9d5d
432	9d5c
433	9d5s
434	9c5h
435	9c5d
436	9c5c
437	9c5s
438	9s5h
439	9s5d
440	9s5c
441	9s5s
442	9h6h
443	9h6d
444	9h6c
445	9h6s
446	9d6h
447	9d6d
448	9d6c
449	9d6s
450	9c6h
451	9c6d
452	9c6c
453	9c6s
454	9s6h
455	9s6d
456	9s6c
457	9s6s
458	9h7h
459	9h7d
460	9h7c
461	9h7s
462	9d7h
463	9d7d
464	9d7c
465	9d7s
466	9c7h
467	9c7d
468	9c7c
469	9c7s
470	9s7h
471	9s7d
472	9s7c
473	9s7s
474	9h8h
475	9h8d
476	9h8c
477	9h8s
478	9d8h
479	9d8d
480	9d8c
481	9d8s
482	9c8h
483	9c8d
484	9c8c
485	9c8s
486	9s8h
487	9s8d
488	9s8c
489	9s8s
490	9h9d
491	9h9c
492	9h9s
493	9d9c
494	9d9s
495	9c9s
496	Th2h
497	Th2d
498	Th2c
499	Th2s
500	Td2h
501	Td2d
502	Td2c
503	Td2s
504	Tc2h
505	Tc2d
506	Tc2c
507	Tc2s
508	Ts2h
509	Ts2d
510	Ts2c
511	Ts2s
512	Th3h
513	Th3d
514	Th3c
515	Th3s
516	Td3h
517	Td3d
518	Td3c
519	Td3s
520	Tc3h
521	Tc3d
522	Tc3c
523	Tc3s
524	Ts3h
525	Ts3d
526	Ts3c
527	Ts3s
528	Th4h
529	Th4d
530	Th4c
531	Th4s
532	Td4h
533	Td4d
534	Td4c
535	Td4s
536	Tc4h
537	Tc4d
538	Tc4c
539	Tc4s
540	Ts4h
541	Ts4d
542	Ts4c
543	Ts4s
544	Th5h
545	Th5d
546	Th5c
547	Th5s
548	Td5h
549	Td5d
550	Td5c
551	Td5s
552	Tc5h
553	Tc5d
554	Tc5c
555	Tc5s
556	Ts5h
557	Ts5d
558	Ts5c
559	Ts5s
560	Th6h
561	Th6d
562	Th6c
563	Th6s
564	Td6h
565	Td6d
566	Td6c
567	Td6s
568	Tc6h
569	Tc6d
570	Tc6c
571	Tc6s
572	Ts6h
573	Ts6d
574	Ts6c
575	Ts6s
576	Th7h
577	Th7d
578	Th7c
579	Th7s
580	Td7h
581	Td7d
582	Td7c
583	Td7s
584	Tc7h
585	Tc7d
586	Tc7c
587	Tc7s
588	Ts7h
589	Ts7d
590	Ts7c
591	Ts7s
592	Th8h
593	Th8d
594	Th8c
595	Th8s
596	Td8h
597	Td8d
598	Td8c
599	Td8s
600	Tc8h
601	Tc8d
602	Tc8c
603	Tc8s
604	Ts8h
605	Ts8d
606	Ts8c
607	Ts8s
608	Th9h
609	Th9d
610	Th9c
611	Th9s
612	Td9h
613	Td9d
614	Td9c
615	Td9s
616	Tc9h
617	Tc9d
618	Tc9c
619	Tc9s
620	Ts9h
621	Ts9d
622	Ts9c
623	Ts9s
624	ThTd
625	ThTc
626	ThTs
627	TdTc
628	TdTs
629	TcTs
630	Jh2h
631	Jh2d
632	Jh2c
633	Jh2s
634	Jd2h
635	Jd2d
636	Jd2c
637	Jd2s
638	Jc2h
639	Jc2d
640	Jc2c
641	Jc2s
642	Js2h
643	Js2d
644	Js2c
645	Js2s
646	Jh3h
647	Jh3d
648	Jh3c
649	Jh3s
650	Jd3h
651	Jd3d
652	Jd3c
653	Jd3s
654	Jc3h
655	Jc3d
656	Jc3c
657	Jc3s
658	Js3h
659	Js3d
660	Js3c
661	Js3s
662	Jh4h
663	Jh4d
664	Jh4c
665	Jh4s
666	Jd4h
667	Jd4d
668	Jd4c
669	Jd4s
670	Jc4h
671	Jc4d
672	Jc4c
673	Jc4s
674	Js4h
675	Js4d
676	Js4c
677	Js4s
678	Jh5h
679	Jh5d
680	Jh5c
681	Jh5s
682	Jd5h
683	Jd5d
684	Jd5c
685	Jd5s
686	Jc5h
687	Jc5d
688	Jc5c
689	Jc5s
690	Js5h
691	Js5d
692	Js5c
693	Js5s
694	Jh6h
695	Jh6d
696	Jh6c
697	Jh6s
698	Jd6h
699	Jd6d
700	Jd6c
701	Jd6s
702	Jc6h
703	Jc6d
704	Jc6c
705	Jc6s
706	Js6h
707	Js6d
708	Js6c
709	Js6s
710	Jh7h
711	Jh7d
712	Jh7c
713	Jh7s
714	Jd7h
715	Jd7d
716	Jd7c
717	Jd7s
718	Jc7h
719	Jc7d
720	Jc7c
721	Jc7s
722	Js7h
723	Js7d
724	Js7c
725	Js7s
726	Jh8h
727	Jh8d
728	Jh8c
729	Jh8s
730	Jd8h
731	Jd8d
732	Jd8c
733	Jd8s
734	Jc8h
735	Jc8d
736	Jc8c
737	Jc8s
738	Js8h
739	Js8d
740	Js8c
741	Js8s
742	Jh9h
743	Jh9d
744	Jh9c
745	Jh9s
746	Jd9h
747	Jd9d
748	Jd9c
749	Jd9s
750	Jc9h
751	Jc9d
752	Jc9c
753	Jc9s
754	Js9h
755	Js9d
756	Js9c
757	Js9s
758	JhTh
759	JhTd
760	JhTc
761	JhTs
762	JdTh
763	JdTd
764	JdTc
765	JdTs
766	JcTh
767	JcTd
768	JcTc
769	JcTs
770	JsTh
771	JsTd
772	JsTc
773	JsTs
774	JhJd
775	JhJc
776	JhJs
777	JdJc
778	JdJs
779	JcJs
780	Qh2h
781	Qh2d
782	Qh2c
783	Qh2s
784	Qd2h
785	Qd2d
786	Qd2c
787	Qd2s
788	Qc2h
789	Qc2d
790	Qc2c
791	Qc2s
792	Qs2h
793	Qs2d
794	Qs2c
795	Qs2s
796	Qh3h
797	Qh3d
798	Qh3c
799	Qh3s
800	Qd3h
801	Qd3d
802	Qd3c
803	Qd3s
804	Qc3h
805	Qc3d
806	Qc3c
807	Qc3s
808	Qs3h
809	Qs3d
810	Qs3c
811	Qs3s
812	Qh4h
813	Qh4d
814	Qh4c
815	Qh4s
816	Qd4h
817	Qd4d
818	Qd4c
819	Qd4s
820	Qc4h
821	Qc4d
822	Qc4c
823	Qc4s
824	Qs4h
825	Qs4d
826	Qs4c
827	Qs4s
828	Qh5h
829	Qh5d
830	Qh5c
831	Qh5s
832	Qd5h
833	Qd5d
834	Qd5c
835	Qd5s
836	Qc5h
837	Qc5d
838	Qc5c
839	Qc5s
840	Qs5h
841	Qs5d
842	Qs5c
843	Qs5s
844	Qh6h
845	Qh6d
846	Qh6c
847	Qh6s
848	Qd6h
849	Qd6d
850	Qd6c
851	Qd6s
852	Qc6h
853	Qc6d
854	Qc6c
855	Qc6s
856	Qs6h
857	Qs6d
858	Qs6c
859	Qs6s
860	Qh7h
861	Qh7d
862	Qh7c
863	Qh7s
864	Qd7h
865	Qd7d
866	Qd7c
867	Qd7s
868	Qc7h
869	Qc7d
870	Qc7c
871	Qc7s
872	Qs7h
873	Qs7d
874	Qs7c
875	Qs7s
876	Qh8h
877	Qh8d
878	Qh8c
879	Qh8s
880	Qd8h
881	Qd8d
882	Qd8c
883	Qd8s
884	Qc8h
885	Qc8d
886	Qc8c
887	Qc8s
888	Qs8h
889	Qs8d
890	Qs8c
891	Qs8s
892	Qh9h
893	Qh9d
894	Qh9c
895	Qh9s
896	Qd9h
897	Qd9d
898	Qd9c
899	Qd9s
900	Qc9h
901	Qc9d
902	Qc9c
903	Qc9s
904	Qs9h
905	Qs9d
906	Qs9c
907	Qs9s
908	QhTh
909	QhTd
910	QhTc
911	QhTs
912	QdTh
913	QdTd
914	QdTc
915	QdTs
916	QcTh
917	QcTd
918	QcTc
919	QcTs
920	QsTh
921	QsTd
922	QsTc
923	QsTs
924	QhJh
925	QhJd
926	QhJc
927	QhJs
928	QdJh
929	QdJd
930	QdJc
931	QdJs
932	QcJh
933	QcJd
934	QcJc
935	QcJs
936	QsJh
937	QsJd
938	QsJc
939	QsJs
940	QhQd
941	QhQc
942	QhQs
943	QdQc
944	QdQs
945	QcQs
946	Kh2h
947	Kh2d
948	Kh2c
949	Kh2s
950	Kd2h
951	Kd2d
952	Kd2c
953	Kd2s
954	Kc2h
955	Kc2d
956	Kc2c
957	Kc2s
958	Ks2h
959	Ks2d
960	Ks2c
961	Ks2s
962	Kh3h
963	Kh3d
964	Kh3c
965	Kh3s
966	Kd3h
967	Kd3d
968	Kd3c
969	Kd3s
970	Kc3h
971	Kc3d
972	Kc3c
973	Kc3s
974	Ks3h
975	Ks3d
976	Ks3c
977	Ks3s
978	Kh4h
979	Kh4d
980	Kh4c
981	Kh4s
982	Kd4h
983	Kd4d
984	Kd4c
985	Kd4s
986	Kc4h
987	Kc4d
988	Kc4c
989	Kc4s
990	Ks4h
991	Ks4d
992	Ks4c
993	Ks4s
994	Kh5h
995	Kh5d
996	Kh5c
997	Kh5s
998	Kd5h
999	Kd5d
1000	Kd5c
1001	Kd5s
1002	Kc5h
1003	Kc5d
1004	Kc5c
1005	Kc5s
1006	Ks5h
1007	Ks5d
1008	Ks5c
1009	Ks5s
1010	Kh6h
1011	Kh6d
1012	Kh6c
1013	Kh6s
1014	Kd6h
1015	Kd6d
1016	Kd6c
1017	Kd6s
1018	Kc6h
1019	Kc6d
1020	Kc6c
1021	Kc6s
1022	Ks6h
1023	Ks6d
1024	Ks6c
1025	Ks6s
1026	Kh7h
1027	Kh7d
1028	Kh7c
1029	Kh7s
1030	Kd7h
1031	Kd7d
1032	Kd7c
1033	Kd7s
1034	Kc7h
1035	Kc7d
1036	Kc7c
1037	Kc7s
1038	Ks7h
1039	Ks7d
1040	Ks7c
1041	Ks7s
1042	Kh8h
1043	Kh8d
1044	Kh8c
1045	Kh8s
1046	Kd8h
1047	Kd8d
1048	Kd8c
1049	Kd8s
1050	Kc8h
1051	Kc8d
1052	Kc8c
1053	Kc8s
1054	Ks8h
1055	Ks8d
1056	Ks8c
1057	Ks8s
1058	Kh9h
1059	Kh9d
1060	Kh9c
1061	Kh9s
1062	Kd9h
1063	Kd9d
1064	Kd9c
1065	Kd9s
1066	Kc9h
1067	Kc9d
1068	Kc9c
1069	Kc9s
1070	Ks9h
1071	Ks9d
1072	Ks9c
1073	Ks9s
1074	KhTh
1075	KhTd
1076	KhTc
1077	KhTs
1078	KdTh
1079	KdTd
1080	KdTc
1081	KdTs
1082	KcTh
1083	KcTd
1084	KcTc
1085	KcTs
1086	KsTh
1087	KsTd
1088	KsTc
1089	KsTs
1090	KhJh
1091	KhJd
1092	KhJc
1093	KhJs
1094	KdJh
1095	KdJd
1096	KdJc
1097	KdJs
1098	KcJh
1099	KcJd
1100	KcJc
1101	KcJs
1102	KsJh
1103	KsJd
1104	KsJc
1105	KsJs
1106	KhQh
1107	KhQd
1108	KhQc
1109	KhQs
1110	KdQh
1111	KdQd
1112	KdQc
1113	KdQs
1114	KcQh
1115	KcQd
1116	KcQc
1117	KcQs
1118	KsQh
1119	KsQd
1120	KsQc
1121	KsQs
1122	KhKd
1123	KhKc
1124	KhKs
1125	KdKc
1126	KdKs
1127	KcKs
1128	Ah2h
1129	Ah2d
1130	Ah2c
1131	Ah2s
1132	Ad2h
1133	Ad2d
1134	Ad2c
1135	Ad2s
1136	Ac2h
1137	Ac2d
1138	Ac2c
1139	Ac2s
1140	As2h
1141	As2d
1142	As2c
1143	As2s
1144	Ah3h
1145	Ah3d
1146	Ah3c
1147	Ah3s
1148	Ad3h
1149	Ad3d
1150	Ad3c
1151	Ad3s
1152	Ac3h
1153	Ac3d
1154	Ac3c
1155	Ac3s
1156	As3h
1157	As3d
1158	As3c
1159	As3s
1160	Ah4h
1161	Ah4d
1162	Ah4c
1163	Ah4s
1164	Ad4h
1165	Ad4d
1166	Ad4c
1167	Ad4s
1168	Ac4h
1169	Ac4d
1170	Ac4c
1171	Ac4s
1172	As4h
1173	As4d
1174	As4c
1175	As4s
1176	Ah5h
1177	Ah5d
1178	Ah5c
1179	Ah5s
1180	Ad5h
1181	Ad5d
1182	Ad5c
1183	Ad5s
1184	Ac5h
1185	Ac5d
1186	Ac5c
1187	Ac5s
1188	As5h
1189	As5d
1190	As5c
1191	As5s
1192	Ah6h
1193	Ah6d
1194	Ah6c
1195	Ah6s
1196	Ad6h
1197	Ad6d
1198	Ad6c
1199	Ad6s
1200	Ac6h
1201	Ac6d
1202	Ac6c
1203	Ac6s
1204	As6h
1205	As6d
1206	As6c
1207	As6s
1208	Ah7h
1209	Ah7d
1210	Ah7c
1211	Ah7s
1212	Ad7h
1213	Ad7d
1214	Ad7c
1215	Ad7s
1216	Ac7h
1217	Ac7d
1218	Ac7c
1219	Ac7s
1220	As7h
1221	As7d
1222	As7c
1223	As7s
1224	Ah8h
1225	Ah8d
1226	Ah8c
1227	Ah8s
1228	Ad8h
1229	Ad8d
1230	Ad8c
1231	Ad8s
1232	Ac8h
1233	Ac8d
1234	Ac8c
1235	Ac8s
1236	As8h
1237	As8d
1238	As8c
1239	As8s
1240	Ah9h
1241	Ah9d
1242	Ah9c
1243	Ah9s
1244	Ad9h
1245	Ad9d
1246	Ad9c
1247	Ad9s
1248	Ac9h
1249	Ac9d
1250	Ac9c
1251	Ac9s
1252	As9h
1253	As9d
1254	As9c
1255	As9s
1256	AhTh
1257	AhTd
1258	AhTc
1259	AhTs
1260	AdTh
1261	AdTd
1262	AdTc
1263	AdTs
1264	AcTh
1265	AcTd
1266	AcTc
1267	AcTs
1268	AsTh
1269	AsTd
1270	AsTc
1271	AsTs
1272	AhJh
1273	AhJd
1274	AhJc
1275	AhJs
1276	AdJh
1277	AdJd
1278	AdJc
1279	AdJs
1280	AcJh
1281	AcJd
1282	AcJc
1283	AcJs
1284	AsJh
1285	AsJd
1286	AsJc
1287	AsJs
1288	AhQh
1289	AhQd
1290	AhQc
1291	AhQs
1292	AdQh
1293	AdQd
1294	AdQc
1295	AdQs
1296	AcQh
1297	AcQd
1298	AcQc
1299	AcQs
1300	AsQh
1301	AsQd
1302	AsQc
1303	AsQs
1304	AhKh
1305	AhKd
1306	AhKc
1307	AhKs
1308	AdKh
1309	AdKd
1310	AdKc
1311	AdKs
1312	AcKh
1313	AcKd
1314	AcKc
1315	AcKs
1316	AsKh
1317	AsKd
1318	AsKc
1319	AsKs
1320	AhAd
1321	AhAc
1322	AhAs
1323	AdAc
1324	AdAs
1325	AcAs

*/