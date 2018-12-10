from sklearn.datasets import load_breast_cancer
from sklearn import tree
import sklearn.model_selection as ms 
import sklearn.metrics as metrics
import graphviz
import matplotlib.pyplot as plt

cancer = load_breast_cancer( )
clf = tree.DecisionTreeClassifier(min_samples_leaf=50)
clf = clf.fit(cancer.data, cancer.target)

dot_data = tree.export_graphviz(clf, out_file=None)
graph = graphviz.Source(dot_data)
graph.render("cancer")

confMatrix = metrics.confusion_matrix(cancer.target, clf.predict(cancer.data))
precision = metrics.precision_score(cancer.target, clf.predict(cancer.data))
recall = metrics.recall_score(cancer.target, clf.predict(cancer.data))
f1 = metrics.f1_score(cancer.target, clf.predict(cancer.data))
kappa = metrics.cohen_kappa_score(cancer.target, clf.predict(cancer.data))

pred_prob = ms.cross_val_predict(clf, cancer.data, cancer.target, cv=10)
confMatrix2 = metrics.confusion_matrix(cancer.target, pred_prob)
precision2 = metrics.precision_score(cancer.target, pred_prob)
recall2 = metrics.recall_score(cancer.target, pred_prob)
f12 = metrics.f1_score(cancer.target, pred_prob)
kappa = metrics.cohen_kappa_score(cancer.target, pred_prob)

tp = [2, 7, 14, 21, 25, 27, 29, 29, 30, 30]
fp = [0, 1, 3, 7, 10, 20, 35, 45, 60, 70]

for val in range(len(tp)):
    tp[val] = tp[val]/30

for val in range(len(fp)):
    fp[val] = fp[val]/70

#plt.plot(fp, tp)
#plt.title("ROC Curve")
#plt.xlabel("FPR")
#plt.ylabel("TPR")
#plt.show( )
    
leaf = [ ]
precision = [ ]
recall = [ ]
f1 = [ ]
kappa = [ ]

for val in arange(1, 10):
    leaf.append(val)
    clf = tree.DecisionTreeClassifier(min_samples_leaf=val)
    clf = clf.fit(cancer.data, cancer.target)
    
    precision.append(metrics.precision_score(cancer.target, clf.predict(cancer.data)))
    recall.append(metrics.recall_score(cancer.target, clf.predict(cancer.data)))
    f1.append(metrics.f1_score(cancer.target, clf.predict(cancer.data)))
    kappa.append(metrics.cohen_kappa_score(cancer.target, clf.predict(cancer.data)))

for val in arange(10, 20, 2):
    leaf.append(val)
    clf = tree.DecisionTreeClassifier(min_samples_leaf=val)
    clf = clf.fit(cancer.data, cancer.target)
    
    precision.append(metrics.precision_score(cancer.target, clf.predict(cancer.data)))
    recall.append(metrics.recall_score(cancer.target, clf.predict(cancer.data)))
    f1.append(metrics.f1_score(cancer.target, clf.predict(cancer.data)))
    kappa.append(metrics.cohen_kappa_score(cancer.target, clf.predict(cancer.data)))
    
for val in arange(20, 50, 5):
    leaf.append(val)
    clf = tree.DecisionTreeClassifier(min_samples_leaf=val)
    clf = clf.fit(cancer.data, cancer.target)
    
    precision.append(metrics.precision_score(cancer.target, clf.predict(cancer.data)))
    recall.append(metrics.recall_score(cancer.target, clf.predict(cancer.data)))
    f1.append(metrics.f1_score(cancer.target, clf.predict(cancer.data)))
    kappa.append(metrics.cohen_kappa_score(cancer.target, clf.predict(cancer.data)))
    
plt.title("Figure for 3b.i")
plt.ylabel("Measurement")
plt.xlabel("Min Leaf Count")
plt.plot(leaf, precision, '-r', leaf, recall, '-b', leaf, f1, '-k', leaf, kappa, '-g')
plt.legend(['Precision', 'Recall', 'F1', 'Kappa'])
plt.plot()