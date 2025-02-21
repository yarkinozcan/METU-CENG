import java.util.ArrayList;

public class ScholarTree {

	public ScholarNode primaryRoot;		//root of the primary B+ tree
	public ScholarNode secondaryRoot;	//root of the secondary B+ tree
	public ScholarTree(Integer order) {
		ScholarNode.order = order;
		primaryRoot = new ScholarNodePrimaryLeaf(null);
		primaryRoot.level = 0;
		secondaryRoot = new ScholarNodeSecondaryLeaf(null);
		secondaryRoot.level = 0;
	}

	public void addPaper(CengPaper paper) {
		primaryRoot = addPaperToPrimaryTree(primaryRoot, paper);

		secondaryRoot = addPaperToSecondaryTree(secondaryRoot, paper);
	}

	private ScholarNode addPaperToPrimaryTree(ScholarNode root, CengPaper paper) {
		if (root.type == ScholarNodeType.Leaf) {
			ScholarNodePrimaryLeaf leafNode = (ScholarNodePrimaryLeaf) root;
			int insertIndex = 0;
			while (insertIndex < leafNode.paperCount() && leafNode.paperIdAtIndex(insertIndex) < paper.paperId()) {
				insertIndex++;
			}
			leafNode.addPaper(insertIndex, paper);

			if (leafNode.paperCount() > 2 * ScholarNode.order) {
				return splitPrimaryLeaf(leafNode);
			}
			return leafNode;

		} else if (root.type == ScholarNodeType.Internal) {
			ScholarNodePrimaryIndex indexNode = (ScholarNodePrimaryIndex) root;
			int childIndex = indexNode.findChildIndexForPaper(paper.paperId());
			ScholarNode updatedChild = addPaperToPrimaryTree(indexNode.getChildrenAt(childIndex), paper);

			if (updatedChild.type == ScholarNodeType.Internal && !indexNode.isChildren(updatedChild)) {
				ScholarNodePrimaryIndex tempUpdated = (ScholarNodePrimaryIndex) updatedChild;
                if (tempUpdated.paperIdCount() > 2 * ScholarNode.order) {
					return tempUpdated.splitPrimaryIndex();
				}
				return updatedChild;
			}
			return indexNode;
		}
		return root;
	}

	private ScholarNode splitPrimaryLeaf(ScholarNodePrimaryLeaf leaf) {
		ScholarNodePrimaryLeaf newLeaf = new ScholarNodePrimaryLeaf(leaf.getParent());

		int midIndex = leaf.paperCount() / 2;

		for (int i = midIndex; i < leaf.paperCount(); i++) {
			newLeaf.addPaper(i - midIndex, leaf.getPapers().get(i));
		}

		while (leaf.paperCount() > midIndex) {
			leaf.getPapers().remove(leaf.paperCount() - 1);
		}

		Integer splitKey = newLeaf.paperIdAtIndex(0);

		if (leaf.getParent() == null) {
			ArrayList<Integer> paperIds = new ArrayList<>();
			ArrayList<ScholarNode> children = new ArrayList<>();
			paperIds.add(splitKey);
			children.add(leaf);
			children.add(newLeaf);

			ScholarNodePrimaryIndex newRoot = new ScholarNodePrimaryIndex(null, paperIds, children);
			leaf.setParent(newRoot);
			newLeaf.setParent(newRoot);
			return newRoot;
		} else {
			ScholarNodePrimaryIndex parent = (ScholarNodePrimaryIndex) leaf.getParent();
			int splitKeyIndex = parent.findChildIndexForPaper(splitKey);
			parent.addKeyAtIndex(splitKey, splitKeyIndex);

			parent.getAllChildren().add(splitKeyIndex + 1, newLeaf);
			newLeaf.setParent(parent);

			if (parent.paperIdCount() > 2 * ScholarNode.order) {
				return parent.splitPrimaryIndex();
			}
			return parent;
		}
	}

	private ScholarNode addPaperToSecondaryTree(ScholarNode root, CengPaper paper) {
		if (root.type == ScholarNodeType.Leaf) {
			ScholarNodeSecondaryLeaf leafNode = (ScholarNodeSecondaryLeaf) root;
			int insertIndex = 0;
			while (insertIndex < leafNode.journalCount() &&
					leafNode.journalAtIndex(insertIndex).compareTo(paper.journal()) < 0) {
				insertIndex++;
			}
			leafNode.addPaper(insertIndex, paper);

			if (leafNode.journalCount() > 2 * ScholarNode.order) {
				return splitSecondaryLeaf(leafNode);
			}
			return leafNode;
		} else if (root.type == ScholarNodeType.Internal) {
			ScholarNodeSecondaryIndex indexNode = (ScholarNodeSecondaryIndex) root;
            int childIndex = indexNode.findChildIndexForJournal(paper.journal());
			ScholarNode updatedChild = addPaperToSecondaryTree(indexNode.getChildrenAt(childIndex), paper);

			if (updatedChild.type == ScholarNodeType.Internal && !indexNode.isChildren(updatedChild)) {
				ScholarNodeSecondaryIndex splitChild = (ScholarNodeSecondaryIndex) updatedChild;
				if (splitChild.journalCount() > 2 * ScholarNode.order) {
					return splitChild.splitSecondaryIndex();
				}
				return updatedChild;
			}
			return indexNode;
		}
		return root;
	}

	private ScholarNode splitSecondaryLeaf(ScholarNodeSecondaryLeaf leaf) {
		ArrayList<String> newLeafJournals = new ArrayList<>();
		ArrayList<ArrayList<Integer>> newLeafPaperIdBucket = new ArrayList<>();

		int midIndex = leaf.journalCount() / 2;

		for (int i = midIndex; i < leaf.journalCount(); i++) {
			newLeafJournals.add(leaf.getJournals().get(i));
			newLeafPaperIdBucket.add(leaf.getPaperIdBucket().get(i));
		}

		int count = leaf.journalCount();

		while (leaf.journalCount() > midIndex) {
			leaf.getJournals().remove(leaf.journalCount() - 1);
			leaf.getPaperIdBucket().remove(--count);
		}

		ScholarNodeSecondaryLeaf newLeaf = new ScholarNodeSecondaryLeaf(leaf.getParent(), newLeafJournals, newLeafPaperIdBucket);

		String splitKey = newLeaf.journalAtIndex(0);

		if (leaf.getParent() == null) {
			ArrayList<String> journals = new ArrayList<>();
			ArrayList<ScholarNode> children = new ArrayList<>();
			journals.add(splitKey);
			children.add(leaf);
			children.add(newLeaf);

			ScholarNodeSecondaryIndex newRoot = new ScholarNodeSecondaryIndex(null, journals, children);

			leaf.setParent(newRoot);
			newLeaf.setParent(newRoot);

			return newRoot;
		} else {
			ScholarNodeSecondaryIndex parent = (ScholarNodeSecondaryIndex) leaf.getParent();
			parent.addChild(splitKey, newLeaf);

			return parent;
		}
	}

	public CengPaper searchPaper(Integer paperId) {
		ScholarNode current = primaryRoot;
		ScholarNodePrimaryIndex indexNode;
		int depth = 0;
		boolean found = false;

		while (current.type == ScholarNodeType.Internal) {
			indexNode = (ScholarNodePrimaryIndex) current;

			System.out.println("\t".repeat(depth) + "<index>");
			for (int i = 0; i < indexNode.paperIdCount(); i++) {
				System.out.println("\t".repeat(depth) + indexNode.paperIdAtIndex(i));
			}
			System.out.println("\t".repeat(depth) + "</index>");

			current = indexNode.getChildrenAt(indexNode.findChildIndexForPaper(paperId));
			depth++;
		}

		ScholarNodePrimaryLeaf leafNode = (ScholarNodePrimaryLeaf) current;

		for (CengPaper paper : leafNode.getPapers()) {
			if (paper.paperId().equals(paperId)) {
				found = true;
			}
		}

		if(found) {
			System.out.println("\t".repeat(depth) + "<data>");
			for (CengPaper paper : leafNode.getPapers()) {
				if (paper.paperId().equals(paperId)) {
					System.out.println("\t".repeat(depth) + "<record>" + paper.fullName() + "</record>");
					System.out.println("\t".repeat(depth) + "</data>");
					return paper;
				}
			}
			System.out.println("\t".repeat(depth) + "</data>");
		}

		System.out.println("Could not find " + paperId);
		return null;
	}

	public void searchJournal(String journal) {
		ScholarNode current = secondaryRoot;
		ScholarNodeSecondaryIndex indexNode;
		int depth = 0;
		boolean found = false;

		while (current.type == ScholarNodeType.Internal) {
			indexNode = (ScholarNodeSecondaryIndex) current;

			System.out.println("\t".repeat(depth) + "<index>");
			for (int i = 0; i < indexNode.journalCount(); i++) {
				System.out.println("\t".repeat(depth) + indexNode.journalAtIndex(i));
			}
			System.out.println("\t".repeat(depth) + "</index>");

			current = indexNode.getChildrenAt(indexNode.findChildIndexForJournal(journal));
			depth++;
		}

		ScholarNodeSecondaryLeaf leafNode = (ScholarNodeSecondaryLeaf) current;

		for (int i = 0; i < leafNode.journalCount(); i++) {
			if (leafNode.journalAtIndex(i).equals(journal)) {
				for (Integer paperId : leafNode.papersAtIndex(i)) {
					CengPaper paper = findPaperById(paperId);
					if (paper != null) {
						found = true;
					}
				}
			}
		}

		if(found) {
			System.out.println("\t".repeat(depth) + "<data>");
			for (int i = 0; i < leafNode.journalCount(); i++) {
				if (leafNode.journalAtIndex(i).equals(journal)) {
					System.out.println("\t".repeat(depth) + journal);
					for (Integer paperId : leafNode.papersAtIndex(i)) {
						CengPaper paper = findPaperById(paperId);
						if (paper != null) {
							System.out.println("\t".repeat(depth + 1) + "<record>" + paper.fullName() + "</record>");
						}
					}
					System.out.println("\t".repeat(depth) + "</data>");
					return;
				}
			}
			System.out.println("\t".repeat(depth) + "</data>");
		}
		System.out.println("Could not find " + journal);
	}


	public void printPrimaryScholar() {
		traversePrimary(primaryRoot, "");
	}

	public void printSecondaryScholar() {
		traverseSecondary(secondaryRoot, "");
	}

	private void traversePrimary(ScholarNode node, String prefix) {
		if (node.type == ScholarNodeType.Internal) {
			ScholarNodePrimaryIndex indexNode = (ScholarNodePrimaryIndex) node;
            System.out.println(prefix + "<index>");
			for (int i = 0; i < indexNode.paperIdCount(); i++) {
				System.out.println(prefix + indexNode.paperIdAtIndex(i));
			}
			System.out.println(prefix + "</index>");
			for (ScholarNode child : indexNode.getAllChildren()) {
				traversePrimary(child, prefix + "\t");
			}
		} else if (node.type == ScholarNodeType.Leaf) {
			ScholarNodePrimaryLeaf leafNode = (ScholarNodePrimaryLeaf) node;
            System.out.println(prefix + "<data>");
			for (CengPaper paper : leafNode.getPapers()) {
				System.out.println(prefix + "<record>" + paper.fullName() + "</record>");
			}
			System.out.println(prefix + "</data>");
		}
	}

	private void traverseSecondary(ScholarNode node, String prefix) {
		if (node.type == ScholarNodeType.Internal) {
			ScholarNodeSecondaryIndex indexNode = (ScholarNodeSecondaryIndex) node;
            System.out.println(prefix + "<index>");
			for (int i = 0; i < indexNode.journalCount(); i++) {
				System.out.println(prefix + indexNode.journalAtIndex(i));
			}
			System.out.println(prefix + "</index>");
			for (ScholarNode child : indexNode.getAllChildren()) {
				traverseSecondary(child, prefix + "\t");
			}
		} else if (node.type == ScholarNodeType.Leaf) {
			ScholarNodeSecondaryLeaf leafNode = (ScholarNodeSecondaryLeaf) node;
            System.out.println(prefix + "<data>");
			for (int i = 0; i < leafNode.journalCount(); i++) {
				System.out.println(prefix + leafNode.journalAtIndex(i));
				for (Integer paperId : leafNode.papersAtIndex(i)) {
					System.out.println(prefix + "\t<record>" + paperId + "</record>");
				}
			}
			System.out.println(prefix + "</data>");
		}
	}

	private CengPaper findPaperById(Integer paperId) {
		ScholarNode current = primaryRoot;
		while (current.type == ScholarNodeType.Internal) {
			ScholarNodePrimaryIndex indexNode = (ScholarNodePrimaryIndex) current;
			current = indexNode.getChildrenAt(indexNode.findChildIndexForPaper(paperId));
		}

		ScholarNodePrimaryLeaf leafNode = (ScholarNodePrimaryLeaf) current;
		for (CengPaper paper : leafNode.getPapers()) {
			if (paper.paperId().equals(paperId)) {
				return paper;
			}
		}
		return null;
	}
}